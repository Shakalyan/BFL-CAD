#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <string>

namespace BFCAD {

    enum class Operation {
        AND,
        OR,
        XOR,
        NOT
    };

    class BooleanFunction {
    private:
        std::unique_ptr<BooleanFunction> left_bf;
        std::unique_ptr<BooleanFunction> right_bf;
        Operation operation;
        std::set<std::string> parameters;

    public:
        BooleanFunction() = delete;
        BooleanFunction(BooleanFunction const&) = delete;
        BooleanFunction& operator=(BooleanFunction const&) = delete;


        BooleanFunction(std::string const& parameter_name);

        BooleanFunction(std::unique_ptr<BooleanFunction> left_bf, std::unique_ptr<BooleanFunction> right_bf, Operation operation);

        bool calculate(std::unordered_map<std::string, bool> const& parameters) const;

        std::string getTruthTable() const;

        void printParameters() const;

    };

}
