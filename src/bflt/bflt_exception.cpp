#include "bflt_exception.h"
#include "formatter.h"
#include <cstring>

BFCAD::BFLTException::BFLTException(BFLTExceptionType err_type, std::string const& fmt, ...)
{
    this->err_type = err_type;
    std::memset(this->err_str, 0, 256);
    BFCAD_fmt(this->err_str, sizeof(this->err_str), fmt);
}

const char* BFCAD::BFLTException::what() const
{
    return this->err_str;
}

BFCAD::BFLTExceptionType BFCAD::BFLTException::get_type() const
{
    return err_type;
}