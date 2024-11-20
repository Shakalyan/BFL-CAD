#include "bflt_visitor.h"

std::any BFCAD::BFLTVisitor::visitProg(BFLParser::ProgContext *ctx) 
{
    std::cout << "start" << std::endl;
    visitChildren(ctx);
    return this->res_bf;
}

std::any BFCAD::BFLTVisitor::visitBlock(BFLParser::BlockContext *ctx) 
{
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

std::any BFCAD::BFLTVisitor::visitAssign(BFLParser::AssignContext *ctx) 
{
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

std::any BFCAD::BFLTVisitor::visitParens(BFLParser::ParensContext *ctx) 
{
    std::cout << "visit parens\n";
    BFCAD::BooleanFunction *bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
    if (static_cast<bool>(ctx->NOT())) {
        bf->invert();
    }
    return bf;
}

std::any BFCAD::BFLTVisitor::visitId(BFLParser::IdContext *ctx) 
{
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

std::any BFCAD::BFLTVisitor::visitOperation(BFLParser::OperationContext *ctx) 
{
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
