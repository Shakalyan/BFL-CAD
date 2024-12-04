#pragma once

#include <vector>
#include <string>

namespace BFCAD
{

    class TruthTable
    {
    public:
        std::vector<std::string> headers;
        std::vector<std::vector<bool>> table;
        int rows_count;
        int columns_count;

    public:

        TruthTable(int rows, int columns);

        std::string to_string() const;

    };

}