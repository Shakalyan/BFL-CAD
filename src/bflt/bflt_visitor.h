#pragma once

#include "generated/BFLBaseVisitor.h"
#include "bf.h"

namespace BFCAD {

    class BFLTVisitor : public BFLBaseVisitor {
    private:
        BFCAD::BooleanFunction *res_bf;
        std::unordered_set<std::string> in_ids;
        std::unordered_map<std::string, BFCAD::BooleanFunction*> bf_ids;
        std::string out_id;

    public:

        virtual std::any visitProg(BFLParser::ProgContext *ctx) override;

        virtual std::any visitBlock(BFLParser::BlockContext *ctx) override;

        virtual std::any visitAssign(BFLParser::AssignContext *ctx) override;

        virtual std::any visitParens(BFLParser::ParensContext *ctx) override;

        virtual std::any visitId(BFLParser::IdContext *ctx) override;

        virtual std::any visitOperation(BFLParser::OperationContext *ctx) override;

    };

}
