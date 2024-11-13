#include <iostream>

#include "ArrayInitLexer.h"
#include "ArrayInitParser.h"
#include "antlr4-runtime.h"
#include "tree/ParseTree.h"

int main()
{
    std::cout << "test" << std::endl;

    antlr4::ANTLRInputStream input(std::cin);

    ArrayInitLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    ArrayInitParser parser(&tokens);

    std::cout << parser.init()->toStringTree(&parser) << std::endl;

    return 0;
}