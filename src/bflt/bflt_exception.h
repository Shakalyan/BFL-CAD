#pragma once

#include <string>

namespace BFCAD {

    enum class BFLTExceptionType {
        INS_ID_ASSIGN,
        ID_NOT_FOUND
    };

    class BFLTException {
    private:
        BFLTExceptionType err_type;
        char err_str[256];

    public:

        BFLTException(BFLTException const&) = delete;
        BFLTException& operator=(BFLTException const&) = delete;

        BFLTException(BFLTExceptionType err_type, std::string const& fmt, ...);

        const char* what() const;

        BFLTExceptionType get_type() const; 

    };

}