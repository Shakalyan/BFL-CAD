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

    class Conjunct {
    public:
        std::unordered_map<std::string, bool> members;

    public:

        Conjunct();
        Conjunct(std::unordered_map<std::string, bool> const& members);

        bool operator==(Conjunct const& conjunct) const;

        std::string toString() const;

        size_t size() const;

    };
    
    class DNF {
    public:
        std::vector<Conjunct> conjuncts;

    public:
        std::string toString() const;

        bool addConjunct(Conjunct const& conjunct);

    };

    class BooleanFunction {
    private:
        std::unique_ptr<BooleanFunction> left_bf;
        std::unique_ptr<BooleanFunction> right_bf;
        Operation operation;
        std::set<std::string> parameters;
        bool is_inverted;

        void move_bf(BooleanFunction *dst, BooleanFunction &&src);

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
        
        BooleanFunction(std::vector<std::unique_ptr<BooleanFunction>> &&bfs, Operation op);

        BooleanFunction(Conjunct const& conjunct);

        BooleanFunction(DNF const& dnf);

        BooleanFunction& operator=(BooleanFunction &&bf);

        void invert();

        bool calculate(std::unordered_map<std::string, bool> const& parameters) const;

        std::string get_truth_table() const;

        void print_parameters() const;

        DNF get_canonical_DNF() const;

    };

}
