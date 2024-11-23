#include "logger.h"
#include "formatter.h"
#include <cstring>

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

void BFCAD::Logger::log(std::string const& text)
{
    Logger &logger = get_instance();
    for (std::ostream *stream : logger.ostreams) {
        *stream << text << "\n";
    }
}