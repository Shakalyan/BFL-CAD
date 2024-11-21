#include <iostream>

#include "bfl_translator.h"
#include "bflt_exception.h"
#include "logger.h"

#include <fstream>



int main()
{
    // BFCAD::BFLTranslator translator;
    // std::cout << translator.translate(std::cin)->getTruthTable() << std::endl;

    // try {
    //     throw BFCAD::BFLTException("Some error", 1);
    // }
    // catch (BFCAD::BFLTException& e) {
    //     std::cout << e.what() << std::endl;
    // }

    // std::ofstream
    // std::cout << __func__ << std::endl;

    std::ofstream log_file;
    log_file.open("log.txt");

    BFCAD::Logger::set_loglevel(BFCAD::LogLevel::HIGH);
    BFCAD::Logger::add_ostream(&std::cout);
    BFCAD::Logger::add_ostream(&log_file);
    //BFCAD::Logger::log("That's some log string\n");
    BFCAD_log("some message");

    log_file.close();

    return 0;
}