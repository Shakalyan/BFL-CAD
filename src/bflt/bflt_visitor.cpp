#include "bflt_visitor.h"
#include "bflt_exception.h"
#include "logger.h"
#include "formatter.h"

std::any BFCAD::BFLTVisitor::visitProg(BFLParser::ProgContext *ctx) 
{
    BFCAD::Logger::log("============ start parsing BFL input ============");
    visitChildren(ctx);
    BFCAD::Logger::log("============= end parsing BFL input =============");
    return this->res_bf;
}

std::any BFCAD::BFLTVisitor::visitBlock(BFLParser::BlockContext *ctx) 
{
    BFCAD::Logger::log(BFCAD::format("visit block: %", ctx->blk->getText()));
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
    return visitChildren(ctx);
}

std::any BFCAD::BFLTVisitor::visitAssign(BFLParser::AssignContext *ctx) 
{
    std::string id = ctx->ID()[0].getText();
    BFCAD::Logger::log(BFCAD::format("visit assign: %", id));
    if (id == out_id) {
        res_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
        return 0;
    }

    if (bf_ids.find(id) != bf_ids.end()) {
        bf_ids[id] = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
    } 
    else if (in_ids.find(id) != in_ids.end()) {
        throw BFCAD::BFLTException(BFCAD::BFLTExceptionType::INS_ID_ASSIGN,
                                   BFCAD::format("assign is not applicable to INS ID(%)", id));
    }
    else {
        throw BFCAD::BFLTException(BFCAD::BFLTExceptionType::ID_NOT_FOUND,
                                   BFCAD::format("ID(%) not found", id));
    }

    return 0;
}

std::any BFCAD::BFLTVisitor::visitParens(BFLParser::ParensContext *ctx) 
{
    BFCAD::Logger::log("visit parens");
    BFCAD::BooleanFunction *bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr()));
    if (static_cast<bool>(ctx->NOT())) {
        bf->invert();
    }
    return bf;
}

std::any BFCAD::BFLTVisitor::visitId(BFLParser::IdContext *ctx) 
{
    std::string id = ctx->ID()->getText();
    BFCAD::Logger::log(BFCAD::format("visit id: ", id));

    BFCAD::BooleanFunction *bf = nullptr;
    if (this->in_ids.find(id) != this->in_ids.end()) {
        bf = new BFCAD::BooleanFunction(id);
    } 
    else if (this->bf_ids.find(id) != this->bf_ids.end()) {
        bf = this->bf_ids[id];
    }
    else {
        throw BFCAD::BFLTException(BFCAD::BFLTExceptionType::ID_NOT_FOUND,
                                   BFCAD::format("ID(%) not found", id));
    }

    if (static_cast<bool>(ctx->NOT())) {
        bf->invert();
    }
    return bf;
}

std::any BFCAD::BFLTVisitor::visitAndOp(BFLParser::AndOpContext *ctx)
{
    BFCAD::Logger::log("visit AND operation");
    BFCAD::BooleanFunction *left_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(0)));
    BFCAD::BooleanFunction *right_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(1)));
    return new BFCAD::BooleanFunction(left_bf, right_bf, BFCAD::Operation::AND);
}

std::any BFCAD::BFLTVisitor::visitOrOp(BFLParser::OrOpContext *ctx)
{
    BFCAD::Logger::log("visit OR operation");
    BFCAD::BooleanFunction *left_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(0)));
    BFCAD::BooleanFunction *right_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(1)));
    return new BFCAD::BooleanFunction(left_bf, right_bf, BFCAD::Operation::OR);
}

std::any BFCAD::BFLTVisitor::visitXorOp(BFLParser::XorOpContext *ctx)
{
    BFCAD::Logger::log("visit XOR operation");
    BFCAD::BooleanFunction *left_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(0)));
    BFCAD::BooleanFunction *right_bf = std::any_cast<BFCAD::BooleanFunction*>(visit(ctx->expr(1)));
    return new BFCAD::BooleanFunction(left_bf, right_bf, BFCAD::Operation::XOR);
}
