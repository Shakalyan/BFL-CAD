#include "bflt_exception.h"

BFCAD::BFLTException::BFLTException(const char *err_str, int err_code)
{
    this->err_code = err_code;
    this->err_str = err_str;
}

const char* BFCAD::BFLTException::what() const
{
    return this->err_str;
}