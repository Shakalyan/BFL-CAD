#include <iostream>
#include <unordered_map>
#include "antlr4-runtime.h"
#include "tree/ParseTree.h"

#include "BFLLexer.h"
#include "BFLParser.h"
#include "BFLBaseVisitor.h"

#include "../bf.h"

class Visitor : public BFLBaseVisitor {
    BFCAD::BooleanFunction *res_bf;

public:

    virtual std::any visitProg(BFLParser::ProgContext *ctx) override {
        std::cout << "start" << std::endl;
        visitChildren(ctx);
        return this->res_bf;
    }

    virtual std::any visitBlock(BFLParser::BlockContext *ctx) override {
        std::cout << "visit block ";
        std::cout << ctx->blk->toString() << std::endl;
        for (auto id : ctx->ID())
            std::cout << id->toString() << std::endl;
        return visitChildren(ctx);
    }

    virtual std::any visitAssign(BFLParser::AssignContext *ctx) override {
        std::cout << "visit assign\n";
        if (ctx->ID()[0].getText() == "RES") {
            res_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
        }
        return 0;
    }

    virtual std::any visitParens(BFLParser::ParensContext *ctx) override {
        std::cout << "visit parens\n";
        return visit(ctx->expr());
    }

    virtual std::any visitId(BFLParser::IdContext *ctx) override {
        std::string id = ctx->ID()->getText();
        std::cout << "visit id: " << id << "\n";
        return new BFCAD::BooleanFunction(id);
    }

    virtual std::any visitOperation(BFLParser::OperationContext *ctx) override {
        std::cout << "visit operation\n";
        BFCAD::BooleanFunction *left_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(0)));
        BFCAD::BooleanFunction *right_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(1)));
        std::cout << "visit operation any cast ok\n";
        BFCAD::Operation op;
        switch (ctx->op->getType()) {
            case BFLParser::AND:
                op = BFCAD::Operation::AND;
                break;
            case BFLParser::OR:
                op = BFCAD::Operation::OR;
                break;
            case BFLParser::XOR:
                op = BFCAD::Operation::XOR;
                break;
        }
        return new BFCAD::BooleanFunction(left_bf, right_bf, op);
    }

};

int main()
{
    std::cout << "test" << std::endl;

    antlr4::ANTLRInputStream input(std::cin);
    BFLLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BFLParser parser(&tokens);
    //std::cout << parser.prog()->toStringTree(&parser) << std::endl;
    Visitor visitor;

    //visitor.visit(parser.prog());
    BFCAD::BooleanFunction *bf = std::any_cast<BFCAD::BooleanFunction*>(visitor.visit(parser.prog()));
    std::cout << bf->getTruthTable() << std::endl;


    // antlr4::tree::ParseTreeWalker walker;
    // MyListener listener;
    // walker.walk(&listener, parser.init());



    return 0;
}