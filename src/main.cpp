#include <iostream>

#include "bfl_translator.h"

int main()
{
    BFCAD::BFLTranslator translator;
    std::cout << translator.translate(std::cin)->getTruthTable() << std::endl;

    return 0;
}