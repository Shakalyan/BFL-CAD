#pragma once

#include "optimizer.h"

namespace BFCAD {

    class QuineOpt : public IOptimizer {
    private:
        DNF get_reduced_DNF(DNF const& canonical_DNF) const;
        DNF get_minimal_DNF(BFCAD::DNF const& canonical_DNF, DNF const& reduced_DNF) const;

    public:
        
        std::unique_ptr<BooleanFunction> optimize(std::unique_ptr<BooleanFunction> bf) override;

    };

}