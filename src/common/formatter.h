#pragma once

#include <cstdarg>
#include <string>
#include <any>
#include <iostream>
#include <vector>

namespace BFCAD {
    static std::string any_string_cast(std::any const& arg)
    {
        try {
            return std::any_cast<std::string>(arg);
        } catch (std::bad_any_cast &e) {
            return std::string(std::any_cast<const char*>(arg));
        }
    }

    template<typename... Args>
    std::string format(std::string const& fmt, Args const& ...args)
    {
        std::vector<std::any> vargs = {args...};
        int vargs_idx = 0;
        size_t prev_subs_idx = 0, subs_idx = 0;
        std::string result;

        while (true) {
            subs_idx = fmt.find('%', prev_subs_idx);
            if (subs_idx == fmt.npos) {
                result.append(fmt.substr(prev_subs_idx, fmt.size()-prev_subs_idx));
                break;
            }
            result.append(fmt.substr(prev_subs_idx, subs_idx-prev_subs_idx));
            result.append(any_string_cast(vargs[vargs_idx++]));
            prev_subs_idx = subs_idx+1;
        }

        return result;
    }
}

