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
        XOR
    };

    class BooleanFunction {
    private:
        std::unique_ptr<BooleanFunction> left_bf;
        std::unique_ptr<BooleanFunction> right_bf;
        Operation operation;
        std::set<std::string> parameters;
        bool is_inverted;

    public:
        BooleanFunction() = delete;
        BooleanFunction(BooleanFunction const&) = delete;
        BooleanFunction& operator=(BooleanFunction const&) = delete;


        BooleanFunction(std::string const& parameter_name, 
                        bool is_inverted = false);

        BooleanFunction(std::unique_ptr<BooleanFunction> left_bf, 
                        std::unique_ptr<BooleanFunction> right_bf, 
                        Operation operation, 
                        bool is_inverted = false);

        BooleanFunction(BooleanFunction *left_bf, 
                        BooleanFunction *right_bf, 
                        Operation operation, 
                        bool is_inverted = false);

        void invert();

        bool calculate(std::unordered_map<std::string, bool> const& parameters) const;

        std::string get_truth_table() const;

        void print_parameters() const;

    };

}
