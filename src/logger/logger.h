#pragma once

#include <vector>
#include <ostream>

namespace BFCAD {

    enum class LogLevel {
        LOW,
        HIGH
    };

    class Logger {
    private:
        std::vector<std::ostream*> ostreams;
        LogLevel level;

        static Logger& get_instance();

        Logger() = default;

    public:
        Logger(Logger const&) = delete;
        Logger& operator=(Logger const&) = delete;

        static void add_ostream(std::ostream *stream);

        static void set_loglevel(LogLevel level);

        static void log(std::string const& text);

    };

    #undef logf

}