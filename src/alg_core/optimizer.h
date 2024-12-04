#pragma once

#include "bf.h"
#include <memory>

namespace BFCAD {

    class IOptimizer {
    public:

        virtual std::unique_ptr<BooleanFunction> optimize(BooleanFunction *bf) = 0; 

    };

}
