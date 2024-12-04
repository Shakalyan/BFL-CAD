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
    return res;
}

BFCAD::Conjunct::Conjunct()
{
    this->members = {};
}

BFCAD::Conjunct::Conjunct(std::unordered_map<std::string, bool> const& members)
{
    this->members = members;
}

size_t BFCAD::Conjunct::size() const
{
    return this->members.size();
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
    return res;
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
    this->operation = Operation::NO_OP;
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

BFCAD::BooleanFunction::BooleanFunction(std::vector<std::unique_ptr<BooleanFunction>> &&bfs, Operation op)
{
    while (bfs.size() != 1) {
        bfs.push_back(std::make_unique<BooleanFunction>(std::move(bfs[0]), std::move(bfs[1]), op));
        bfs.erase(bfs.begin());
        bfs.erase(bfs.begin());
    }
    move_bf(this, std::move(*bfs[0]));
}

BFCAD::BooleanFunction::BooleanFunction(Conjunct const& conjunct)
{
    std::vector<std::unique_ptr<BooleanFunction>> bfs;
    for (auto const& mmb : conjunct.members) {
        bfs.push_back(std::make_unique<BooleanFunction>(mmb.first, !mmb.second));
    }
    move_bf(this, BooleanFunction(std::move(bfs), Operation::AND));
}

BFCAD::BooleanFunction::BooleanFunction(DNF const& dnf)
{
    std::vector<std::unique_ptr<BooleanFunction>> bfs;
    for (auto const& conj : dnf.conjuncts) {
        bfs.push_back(std::make_unique<BooleanFunction>(conj));
    }
    move_bf(this, BooleanFunction(std::move(bfs), Operation::OR));
    //std::cout << bfs[2]->get_truth_table() << std::endl;
}

BFCAD::BooleanFunction& BFCAD::BooleanFunction::operator=(BooleanFunction&& bf)
{
    move_bf(this, std::move(bf));
    return *this;
}

void BFCAD::BooleanFunction::move_bf(BooleanFunction *dst, BooleanFunction &&src)
{
    dst->left_bf = std::move(src.left_bf);
    dst->right_bf = std::move(src.right_bf);
    dst->operation = src.operation;
    dst->parameters = std::move(src.parameters);
    dst->is_inverted = src.is_inverted;
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

std::unique_ptr<BFCAD::TruthTable> BFCAD::BooleanFunction::get_truth_table() const
{
    int columns = this->parameters.size() + 1;
    int rows = (1 << (columns-1));
    std::unique_ptr<TruthTable> truth_table = std::make_unique<TruthTable>(rows, columns);

    std::unordered_map<std::string, bool> params;
    int header_idx = 0;
    for (auto const& param : this->parameters) {
        truth_table->headers[header_idx++] = param;
        params.insert(std::make_pair(param, 0));
    }
    truth_table->headers[header_idx] = "result";    

    for (int i = 0; i < rows; ++i) {
        int ofs = this->parameters.size()-1;
        int col = 0;
        for (auto it = this->parameters.begin(); it != this->parameters.end(); ++it) {
            bool value = i & 1<<ofs;
            params[*it] = value;
            truth_table->table[i][col] = value;
            --ofs;
            ++col;
        }

        bool bf_result = this->calculate(params);
        truth_table->table[i][columns-1] = bf_result;
    }

    return truth_table;
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

    return dnf;
}

std::string BFCAD::BooleanFunction::to_string() const
{
    std::string result;

    if (!this->left_bf && !this->right_bf) {
        if (this->is_inverted)
            result.append("!");
        result.append(*this->parameters.begin());
        return result;
    }

    if (this->is_inverted)
        result.append("!(");

    std::string lbf_res = this->left_bf->to_string();
    if (this->operation < this->left_bf->operation && !this->left_bf->is_inverted) {
        result.append("(" + lbf_res + ")");
    } else {
        result.append(lbf_res);
    }

    switch(this->operation) {
        case Operation::AND:
            result.append(" && ");
            break;
        case Operation::OR:
            result.append(" || ");
            break;
        case Operation::XOR:
            result.append(" ^ ");
            break;
    }

    std::string rbf_res = this->right_bf->to_string();
    if (this->operation < this->right_bf->operation && !this->right_bf->is_inverted) {
        result.append("(" + rbf_res + ")");
    } else {
        result.append(rbf_res);
    }

    if (this->is_inverted)
        result.append(")");

    return result;
}
