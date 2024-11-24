#include "bf.h"
#include "logger.h"

#include <iostream>

bool BFCAD::Conjunct::operator==(Conjunct const& conjunct) const
{
    if (this->members.size() != conjunct.members.size())
        return false;
    for (auto const& mmb : this->members) {
        auto it = conjunct.members.find(mmb.first);
        if (it == conjunct.members.end())
            return false;
        if (mmb.second != it->second)
            return false;
    }
    return true;
}

std::string BFCAD::Conjunct::toString() const
{
    std::string res;
    for (auto const& mmb : this->members) {
        if (!mmb.second)
            res.append("!");
        res.append(mmb.first);
    }
    return std::move(res);
}

BFCAD::Conjunct::Conjunct()
{
    this->members = {};
}

BFCAD::Conjunct::Conjunct(std::unordered_map<std::string, bool> const& members)
{
    this->members = members;
}

std::string BFCAD::DNF::toString() const
{
    std::string res;
    size_t conj_num = this->conjuncts.size();
    for (int i = 0; i < conj_num; ++i) {
        res.append(this->conjuncts[i].toString());
        if (i != conj_num-1)
            res.append(" || ");
    }
    return std::move(res);
}

bool BFCAD::DNF::addConjunct(Conjunct const& conjunct)
{
    for (Conjunct const& c : this->conjuncts) {
        if (c == conjunct)
            return false;
    }
    this->conjuncts.push_back(conjunct);
    return true;
}

BFCAD::BooleanFunction::BooleanFunction(std::string const& parameter_name, 
                                        bool is_inverted) 
{
    this->parameters.insert(parameter_name);
    this->is_inverted = is_inverted;
}

BFCAD::BooleanFunction::BooleanFunction(std::unique_ptr<BooleanFunction> left_bf, 
                                        std::unique_ptr<BooleanFunction> right_bf, 
                                        Operation operation,
                                        bool is_inverted) 
{
    this->left_bf = std::move(left_bf);
    this->right_bf = std::move(right_bf);
    this->operation = operation;
    this->is_inverted = is_inverted;
    this->parameters.insert(this->left_bf->parameters.begin(), this->left_bf->parameters.end());
    this->parameters.insert(this->right_bf->parameters.begin(), this->right_bf->parameters.end());
}

BFCAD::BooleanFunction::BooleanFunction(BooleanFunction *left_bf, 
                                        BooleanFunction *right_bf, 
                                        Operation operation,
                                        bool is_inverted)
{
    this->left_bf = std::unique_ptr<BooleanFunction>(left_bf);
    this->right_bf = std::unique_ptr<BooleanFunction>(right_bf);
    this->operation = operation;
    this->is_inverted = is_inverted;
    this->parameters.insert(this->left_bf->parameters.begin(), this->left_bf->parameters.end());
    this->parameters.insert(this->right_bf->parameters.begin(), this->right_bf->parameters.end());    
}

void BFCAD::BooleanFunction::invert()
{
    this->is_inverted = !this->is_inverted;
}

bool BFCAD::BooleanFunction::calculate(std::unordered_map<std::string, bool> const& parameters) const 
{
    if (this->left_bf == NULL && this->right_bf == NULL) {
        return (parameters.find(*this->parameters.begin())->second) ^ this->is_inverted; // check find
    }

    bool left_value = this->left_bf->calculate(parameters);
    bool right_value = this->right_bf->calculate(parameters);

    switch (this->operation) {
        case Operation::AND:
            return (left_value && right_value) ^ this->is_inverted;
        case Operation::OR:
            return (left_value || right_value) ^ this->is_inverted;
        case Operation::XOR:
            return (left_value ^ right_value) ^ this->is_inverted;
    }
    return 0;
}

std::string BFCAD::BooleanFunction::get_truth_table() const
{
    std::string result;
    std::unordered_map<std::string, bool> params;

    for (auto const& param : this->parameters) {
        result.append("\t" + param);
        params.insert(std::make_pair(param, 0));
    }
    result.append("\tresult");

    int limit = (1<<this->parameters.size());
    for (int i = 0; i < limit; ++i) {
        result.append("\n");

        int param_idx = this->parameters.size()-1;
        for (auto it = this->parameters.begin(); it != this->parameters.end(); ++it) {
            bool value = i & 1<<param_idx;
            params[*it] = value;
            --param_idx;
            result.append("\t").append(std::to_string(value));
        }

        bool bf_result = this->calculate(params);
        result.append("\t").append(std::to_string(bf_result));
    }

    return result;
}

void BFCAD::BooleanFunction::print_parameters() const
{
    for (auto const& param : this->parameters) {
        BFCAD::Logger::log(std::string(param));
    }
}

BFCAD::DNF BFCAD::BooleanFunction::get_canonical_DNF() const
{
    DNF dnf;
    std::unordered_map<std::string, bool> params;

    for (auto const& param : this->parameters) {
        params.insert(std::make_pair(param, 0));
    }

    int limit = (1<<this->parameters.size());
    for (int i = 0; i < limit; ++i) {
        int param_idx = this->parameters.size()-1;
        for (auto it = this->parameters.begin(); it != this->parameters.end(); ++it) {
            bool value = i & 1<<param_idx;
            params[*it] = value;
            --param_idx;
        }
        bool bf_result = this->calculate(params);
        if (bf_result) {
            dnf.conjuncts.emplace_back(params);            
        }
    }

    return std::move(dnf);
}
