#include "logger.h"
#include "formatter.h"
#include <cstring>

BFCAD::Logger& BFCAD::Logger::get_instance()
{
    static Logger logger;
    std::memset(logger.buffer, 0, 256);
    return logger;
}

void BFCAD::Logger::add_ostream(std::ostream *stream)
{
    get_instance().ostreams.push_back(stream);
}

void BFCAD::Logger::set_loglevel(LogLevel level)
{
    get_instance().level = level;
}

void BFCAD::Logger::log(const char *func_name, std::string const& fmt, ...)
{
    Logger &logger = get_instance();
    BFCAD_fmt(logger.buffer, sizeof(logger.buffer), fmt);

    for (std::ostream *stream : logger.ostreams) {
        if (logger.level == LogLevel::HIGH)
            *stream << func_name << "(): ";
        *stream << logger.buffer << "\n";
    }
}