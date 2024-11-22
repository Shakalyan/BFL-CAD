#pragma once

#include <cstdarg>
#include <string>

#define BFCAD_fmt(_buf, _max_size, _fmt) {                  \
    std::va_list _args;                                     \
    va_start(_args, _fmt);                                  \
    std::vsnprintf(_buf, _max_size, _fmt.c_str(), _args);   \
    va_end(_args);                                          \
}


