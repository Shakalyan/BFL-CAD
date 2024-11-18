#include <iostream>

#include "bf.h"

int main()
{
    // (x1 | x2) & x3
    std::unordered_map<std::string, bool> parameters;
    parameters.emplace("x1", 1);
    parameters.emplace("x2", 1);
    parameters.emplace("x3", 0);

    std::unique_ptr<BFCAD::BooleanFunction> x1_bf = std::make_unique<BFCAD::BooleanFunction>("x1");
    std::unique_ptr<BFCAD::BooleanFunction> x2_bf = std::make_unique<BFCAD::BooleanFunction>("x2");
    std::unique_ptr<BFCAD::BooleanFunction> x3_bf = std::make_unique<BFCAD::BooleanFunction>("x3");

    std::unique_ptr<BFCAD::BooleanFunction> x1_or_x2_bf = std::make_unique<BFCAD::BooleanFunction>(std::move(x1_bf), std::move(x2_bf), BFCAD::Operation::OR);
    std::unique_ptr<BFCAD::BooleanFunction> final_bf = std::make_unique<BFCAD::BooleanFunction>(std::move(x1_or_x2_bf), std::move(x3_bf), BFCAD::Operation::AND);

    // final_bf->printParameters();
    // std::cout << final_bf->calculate(parameters) << std::endl;

    std::cout << final_bf->getTruthTable() << std::endl;


    return 0;
}