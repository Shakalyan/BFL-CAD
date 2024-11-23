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
        std::string err_message;

    public:

        BFLTException(BFLTException const&) = delete;
        BFLTException& operator=(BFLTException const&) = delete;

        BFLTException(BFLTExceptionType err_type, std::string const& err_message);

        std::string what() const;

        BFLTExceptionType get_type() const; 

    };

}