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
            std::cout << "\t" << implicant_mx[impl_i][conj_i];
        }
        std::cout << "\n";
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

    


    return minimal_dnf;
}

std::unique_ptr<BFCAD::BooleanFunction> BFCAD::QuineOpt::optimize(std::unique_ptr<BFCAD::BooleanFunction> bf)
{
    DNF cdnf = bf->get_canonical_DNF();
    BFCAD::Logger::log(cdnf.toString());
    DNF rdnf = get_reduced_DNF(cdnf);
    BFCAD::Logger::log(rdnf.toString());
    DNF minimal_dnf = get_minimal_DNF(cdnf, rdnf);
    return std::unique_ptr<BFCAD::BooleanFunction>(nullptr);
}

