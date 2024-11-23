#include "bflt_exception.h"
#include "formatter.h"
#include <cstring>

BFCAD::BFLTException::BFLTException(BFLTExceptionType err_type, std::string const& err_message)
{
    this->err_type = err_type;
    this->err_message = err_message;
}

std::string BFCAD::BFLTException::what() const
{
    return this->err_message;
}

BFCAD::BFLTExceptionType BFCAD::BFLTException::get_type() const
{
    return err_type;
}