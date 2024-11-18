#include "bf.h"
#include <iostream>

BFCAD::BooleanFunction::BooleanFunction(std::string const& parameter_name) 
{
    this->parameters.insert(parameter_name);
}

BFCAD::BooleanFunction::BooleanFunction(std::unique_ptr<BooleanFunction> left_bf, 
                                        std::unique_ptr<BooleanFunction> right_bf, 
                                        Operation operation) 
{
    this->left_bf = std::move(left_bf);
    this->right_bf = std::move(right_bf);
    this->operation = operation;
    this->parameters.insert(this->left_bf->parameters.begin(), this->left_bf->parameters.end());
    this->parameters.insert(this->right_bf->parameters.begin(), this->right_bf->parameters.end());
}

bool BFCAD::BooleanFunction::calculate(std::unordered_map<std::string, bool> const& parameters) const 
{
    if (this->left_bf == NULL && this->right_bf == NULL) {
        return parameters.find(*this->parameters.begin())->second; // check find
    }

    bool left_value = this->left_bf->calculate(parameters);
    if (this->operation == Operation::NOT) {
        return !left_value;
    }
    
    bool right_value = this->right_bf->calculate(parameters);        

    switch (this->operation) {
        case Operation::AND:
            return left_value && right_value;
        case Operation::OR:
            return left_value || right_value;
        case Operation::XOR:
            return left_value ^ right_value;
    }
    return 0;
}

std::string BFCAD::BooleanFunction::getTruthTable() const
{
    std::string result;
    std::unordered_map<std::string, bool> params;

    for (auto param : this->parameters) {
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

void BFCAD::BooleanFunction::printParameters() const
{
    for (auto param : this->parameters) {
        std::cout << param << std::endl;
    }
}
