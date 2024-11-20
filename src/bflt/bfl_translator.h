#pragma once

#include <memory>

#include "bflt_visitor.h"

namespace BFCAD {

    class BFLTranslator {
    private:
        BFLTVisitor visitor;

    public:
        
        BFLTranslator(BFLTranslator const&) = delete;
        BFLTranslator& operator=(BFLTranslator const&) = delete;

        BFLTranslator();

        std::unique_ptr<BFCAD::BooleanFunction> translate(std::istream &istream);

    };

}
