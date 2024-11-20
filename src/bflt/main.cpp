#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "antlr4-runtime.h"
#include "tree/ParseTree.h"

#include "BFLLexer.h"
#include "BFLParser.h"
#include "BFLBaseVisitor.h"

#include "../bf.h"

class Visitor : public BFLBaseVisitor {
private:
    BFCAD::BooleanFunction *res_bf;
    std::unordered_set<std::string> in_ids;
    std::unordered_map<std::string, BFCAD::BooleanFunction*> bf_ids;
    std::string out_id;

public:

    virtual std::any visitProg(BFLParser::ProgContext *ctx) override {
        std::cout << "start" << std::endl;
        visitChildren(ctx);
        return this->res_bf;
    }

    virtual std::any visitBlock(BFLParser::BlockContext *ctx) override {
        std::cout << "visit block ";
        std::cout << ctx->blk->toString() << std::endl;
        switch (ctx->blk->getType()) {
            case BFLParser::INS:
                for (auto id : ctx->ID()) in_ids.insert(id->getText());
                break;
            case BFLParser::BFS:
                for (auto id : ctx->ID()) bf_ids[id->getText()] = nullptr;
                break;
            case BFLParser::OUT:
                out_id = ctx->ID()[0]->getText();
                break;
        }
        // for (auto id : ctx->ID())
        //     std::cout << id->toString() << std::endl;
        return visitChildren(ctx);
    }

    virtual std::any visitAssign(BFLParser::AssignContext *ctx) override {
        std::cout << "visit assign\n";
        std::string id = ctx->ID()[0].getText();
        if (id == out_id) {
            res_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
            return 0;
        }

        if (bf_ids.find(id) != bf_ids.end()) {
            bf_ids[id] = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
        } else {
            std::cout << "BF NOT FOUND EXCEPTION\n";
            return 0;
        }

        return 0;
    }

    virtual std::any visitParens(BFLParser::ParensContext *ctx) override {
        std::cout << "visit parens\n";
        BFCAD::BooleanFunction *bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
        if (static_cast<bool>(ctx->NOT())) {
            bf->invert();
        }
        return bf;
    }

    virtual std::any visitId(BFLParser::IdContext *ctx) override {
        std::string id = ctx->ID()->getText();
        std::cout << "visit id: " << id << "\n";

        BFCAD::BooleanFunction *bf = nullptr;
        if (this->in_ids.find(id) != this->in_ids.end()) {
            bf = new BFCAD::BooleanFunction(id);
        } 
        else if (this->bf_ids.find(id) != this->bf_ids.end()) {
            bf = this->bf_ids[id];
        }
        else {
            std::cout << "ID NOT FOUND EXCEPTION\n";
        }

        if (static_cast<bool>(ctx->NOT())) {
            bf->invert();
        }
        return bf;
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