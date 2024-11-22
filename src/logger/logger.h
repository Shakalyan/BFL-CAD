#pragma once

#include <vector>
#include <ostream>

#define BFCAD_log(fmt, ...) {BFCAD::Logger::log(__func__, fmt, ##__VA_ARGS__);}

namespace BFCAD {

    enum class LogLevel {
        LOW,
        HIGH
    };

    class Logger {
    private:
        std::vector<std::ostream*> ostreams;
        LogLevel level;
        char buffer[256];

        static Logger& get_instance();

        Logger() = default;

    public:
        Logger(Logger const&) = delete;
        Logger& operator=(Logger const&) = delete;

        static void add_ostream(std::ostream *stream);

        static void set_loglevel(LogLevel level);

        static void log(const char *func_name, std::string const& fmt, ...);

    };

    #undef logf

}