#include "bfl_translator.h"
#include "generated/BFLLexer.h"
#include "generated/BFLParser.h"

BFCAD::BFLTranslator::BFLTranslator()
{
    this->visitor = BFLTVisitor();
}

std::unique_ptr<BFCAD::BooleanFunction> BFCAD::BFLTranslator::translate(std::string const& input)
{
    antlr4::ANTLRInputStream input_stream(input);
    BFLLexer lexer(&input_stream);
    antlr4::CommonTokenStream tokens(&lexer);
    BFLParser parser(&tokens);

    BFCAD::BooleanFunction *bf = std::any_cast<BFCAD::BooleanFunction*>(this->visitor.visit(parser.prog()));
    return std::unique_ptr<BFCAD::BooleanFunction>(bf);
}