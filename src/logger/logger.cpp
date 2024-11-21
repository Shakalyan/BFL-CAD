#include "logger.h"

BFCAD::Logger& BFCAD::Logger::get_instance()
{
    static Logger logger;
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

void BFCAD::Logger::log(const char *str, const char *func_name)
{
    Logger &logger = get_instance();
    for (std::ostream *stream : logger.ostreams) {
        if (logger.level == LogLevel::HIGH)
            *stream << func_name << ": ";
        *stream << str << "\n";
    }
}