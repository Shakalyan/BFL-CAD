#pragma once

#include "bf.h"
#include <memory>

namespace BFCAD {

    class IOptimizer {
    public:

        virtual std::unique_ptr<BooleanFunction> optimize(std::unique_ptr<BooleanFunction> bf) = 0; 

    };

}
