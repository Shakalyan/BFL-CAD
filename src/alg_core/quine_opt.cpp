#include "quine_opt.h"
#include "logger.h"
#include "formatter.h"

#include <unistd.h> //=============================

static BFCAD::Conjunct merge(BFCAD::Conjunct const& conj1, BFCAD::Conjunct const& conj2)
{
    BFCAD::Conjunct res;

    if (conj1.members.size() != conj2.members.size())
        return BFCAD::Conjunct();
    
    std::string merging_id;
    for (auto const& mmb1 : conj1.members) {
        auto mmb2_it = conj2.members.find(mmb1.first);
        if (mmb2_it == conj2.members.end())
            return BFCAD::Conjunct();
        if (mmb1.second != mmb2_it->second) {
            if (merging_id != "")
                return BFCAD::Conjunct();
            merging_id = mmb1.first;
        } else {
            res.members.insert(mmb1);
        }
    }

    return std::move(res);
}

static bool is_overlapping(BFCAD::Conjunct const& implicant, BFCAD::Conjunct const& conjunct)
{
    for (auto const& i_mmb : implicant.members) {
        auto conj_mmb_it = conjunct.members.find(i_mmb.first);
        if (conj_mmb_it == conjunct.members.end())
            return false;
        if (i_mmb.second != conj_mmb_it->second)
            return false;
    }
    return true;
}

static bool reduce_DNF(BFCAD::DNF const& dnf, BFCAD::DNF &reduced_dnf)
{
    size_t dnf_size = dnf.conjuncts.size();
    std::vector<bool> merged(dnf_size);

    for (int l = 0; l < dnf_size; ++l) {
        for (int r = l+1; r < dnf_size; ++r) {
            BFCAD::Conjunct merged_conj = merge(dnf.conjuncts[l], dnf.conjuncts[r]);
            if (merged_conj.members.size()) {
                merged[l] = true;
                merged[r] = true;
                reduced_dnf.addConjunct(merged_conj);
            }
        }
        if (!merged[l]) {
            reduced_dnf.addConjunct(dnf.conjuncts[l]);
        }
    }
    
    for (bool b : merged) {
        if (b)
            return true;
    }
    return false;
}

BFCAD::DNF BFCAD::QuineOpt::get_reduced_DNF(BFCAD::DNF const& canonical_DNF) const
{
    DNF dnf = canonical_DNF, reduced_dnf;
    while (reduce_DNF(dnf, reduced_dnf)) {
        dnf = reduced_dnf;
        reduced_dnf.conjuncts.clear();
    }
    return std::move(reduced_dnf);
}

BFCAD::DNF BFCAD::QuineOpt::get_minimal_DNF(BFCAD::DNF const& canonical_DNF, BFCAD::DNF const& reduced_dnf) const
{
    DNF minimal_dnf;
    size_t cdnf_conj_num = canonical_DNF.conjuncts.size();
    size_t rdnf_impl_num = reduced_dnf.conjuncts.size();

    // build implicant matrix
    std::vector<std::vector<bool>> implicant_mx;
    for (int impl_i = 0; impl_i < rdnf_impl_num; ++impl_i) {
        implicant_mx.push_back(std::vector<bool>(cdnf_conj_num));
        for (int conj_i = 0; conj_i < cdnf_conj_num; ++conj_i) {
            implicant_mx[impl_i][conj_i] = is_overlapping(reduced_dnf.conjuncts[impl_i], canonical_DNF.conjuncts[conj_i]);
            //std::cout << "\t" << implicant_mx[impl_i][conj_i];
        }
        //std::cout << "\n";
    }

    // find core implicants
    std::vector<bool> core_impl(rdnf_impl_num);
    for (int conj_i = 0; conj_i < cdnf_conj_num; ++conj_i) {
        int overlapped_impl_idx = -1;
        for (int impl_i = 0; impl_i < rdnf_impl_num; ++impl_i) {
            if (implicant_mx[impl_i][conj_i]) {
                if (overlapped_impl_idx != -1) {
                    overlapped_impl_idx = -1;
                    break;
                }
                overlapped_impl_idx = impl_i;                
            }
        }
        if (overlapped_impl_idx != -1)
            core_impl[overlapped_impl_idx] = true;
    }

    // find not overlapped conjunct indices
    std::vector<int> not_overlapped_conjs_idxs;
    for (int conj_i = 0; conj_i < cdnf_conj_num; ++conj_i) {
        bool overlapped = false;
        for (int impl_i = 0; impl_i < rdnf_impl_num; ++impl_i) {
            if (core_impl[impl_i] && implicant_mx[impl_i][conj_i]) {
                overlapped = true;
                break;
            }
        }
        if (!overlapped) {
            not_overlapped_conjs_idxs.push_back(conj_i);
            //std::cout << conj_i << " ";
        }
    }

    // find not core implicants indices
    std::vector<int> not_core_impls_idxs;
    for (int i = 0; i < rdnf_impl_num; ++i) {
        if (!core_impl[i])
            not_core_impls_idxs.push_back(i);
    }

    // find minimal needed non-core implicants
    std::vector<Conjunct> optimal_conjuncts;
    size_t minimal_conjs_size = 0;
    for (int idx : not_core_impls_idxs) {
        minimal_conjs_size += reduced_dnf.conjuncts[idx].size();
    }

    size_t nc_impls_num = not_core_impls_idxs.size();
    size_t no_conjs_num = not_overlapped_conjs_idxs.size();
    int limit = (1 << nc_impls_num);
    for (int bm = 0; bm < limit; ++bm) {
        size_t conjs_size = 0;
        std::vector<bool> used_impls(nc_impls_num);
        for (int i = 0; i < nc_impls_num; ++i) {
            used_impls[i] = static_cast<bool>(bm & 1<<i);
            if (used_impls[i])
                conjs_size += reduced_dnf.conjuncts[not_core_impls_idxs[i]].size();
        }

        if (conjs_size >= minimal_conjs_size)
            continue;

        std::vector<bool> overlapped_conjs(no_conjs_num);
        for (int conj_idx = 0; conj_idx < no_conjs_num; ++conj_idx) {
            for (int impl_idx = 0; impl_idx < nc_impls_num; ++impl_idx) {
                if (used_impls[impl_idx] && implicant_mx[not_core_impls_idxs[impl_idx]][not_overlapped_conjs_idxs[conj_idx]]) {
                    overlapped_conjs[conj_idx] = true;
                    break;
                }
            }
        }

        bool overlapped_all = true;
        for (bool overlapped : overlapped_conjs) {
            if (!overlapped) {
                overlapped_all = false;
                break;
            }
        }
        if (overlapped_all) {
            optimal_conjuncts.clear();
            minimal_conjs_size = conjs_size;
            for (int i = 0; i < nc_impls_num; ++i) {
                if (used_impls[i])
                    optimal_conjuncts.push_back(reduced_dnf.conjuncts[not_core_impls_idxs[i]]);
            }
        }
    }

    for (int i = 0; i < rdnf_impl_num; ++i) {
        if (core_impl[i])
            minimal_dnf.addConjunct(reduced_dnf.conjuncts[i]);
    }

    for (auto const& conj : optimal_conjuncts) {
        minimal_dnf.addConjunct(conj);
    }

    return minimal_dnf;
}

std::unique_ptr<BFCAD::BooleanFunction> BFCAD::QuineOpt::optimize(BooleanFunction *bf)
{
    DNF cdnf = bf->get_canonical_DNF();
    //BFCAD::Logger::log(cdnf.toString());
    DNF rdnf = get_reduced_DNF(cdnf);
    //BFCAD::Logger::log(rdnf.toString());
    DNF minimal_dnf = get_minimal_DNF(cdnf, rdnf);
    //std::cout << minimal_dnf.toString() << std::endl;
    return std::make_unique<BooleanFunction>(minimal_dnf);
}

