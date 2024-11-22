#include <iostream>
#include <fstream>

#include "bfl_translator.h"
#include "bflt_exception.h"
#include "logger.h"

#include <cstdarg>
std::string bvsrprintf(std::string const& fmt, std::va_list args)
{
    std::string s = *va_arg(args, std::string*);
    std::cout << s << std::endl;
    return std::string("asdf");
}

void log(std::string const& fmt, ...)
{
    std::va_list args;
    va_start(args, fmt);
    bvsrprintf(fmt, args);
    va_end(args);
}

int main()
{
    std::ofstream log_file;
    log_file.open("log.txt");
    BFCAD::Logger::set_loglevel(BFCAD::LogLevel::LOW);
    BFCAD::Logger::add_ostream(&std::cout);
    BFCAD::Logger::add_ostream(&log_file);

    BFCAD_log("Program start");

    // BFCAD::BFLTranslator translator;
    // try {
    //     std::unique_ptr<BFCAD::BooleanFunction> bf = translator.translate(std::cin);
    //     BFCAD_log(bf->get_truth_table());
    // }
    // catch (BFCAD::BFLTException &e) {
    //     BFCAD_log(e.what());
    // }
    std::string test("asdf123123");
    log("test %s %d", test, "kek");

    BFCAD_log("Program end");

    log_file.close();
    return 0;
}