#include "truth_table.h"

#include <iostream>

BFCAD::TruthTable::TruthTable(int rows, int columns) :  headers(columns), table(rows, std::vector<bool>(columns)),
                                                        rows_count(rows), columns_count(columns)
{
    
}

std::string BFCAD::TruthTable::to_string() const
{
    std::string result;
    
    for (std::string const& header : this->headers)
        result.append("\t").append(header);

    for (int r = 0; r < this->rows_count; ++r) {
        result.append("\n");
        for (int c = 0; c < this->columns_count; ++c) {
            result.append("\t").append(std::to_string(this->table[r][c]));
        }
    }

    return result;
}
