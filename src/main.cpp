#include <iostream>
#include <fstream>

#include "bfl_translator.h"
#include "bflt_exception.h"
#include "logger.h"
#include "formatter.h"
#include "optimizer.h"
#include "quine_opt.h"

#include "bfc_gui.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    std::ofstream log_file;
    log_file.open("log.txt");
    BFCAD::Logger::set_loglevel(BFCAD::LogLevel::LOW);
    BFCAD::Logger::add_ostream(&std::cout);
    BFCAD::Logger::add_ostream(&log_file);

    BFCAD::UI::BfcGui gui;
    gui.showMaximized();

    BFCAD::Logger::log("Program start");

    // BFCAD::BFLTranslator translator;
    // try {
    //     std::unique_ptr<BFCAD::BooleanFunction> bf = translator.translate(std::cin);
    //     BFCAD::Logger::log(bf->get_truth_table());
        
    //     BFCAD::IOptimizer&& optimizer = BFCAD::QuineOpt();
    //     std::unique_ptr<BFCAD::BooleanFunction> optimized_bf = optimizer.optimize(std::move(bf));
    //     BFCAD::Logger::log(optimized_bf->get_truth_table());
    // }
    // catch (BFCAD::BFLTException &e) {
    //     BFCAD::Logger::log(BFCAD::format("EXCEPTION: %", e.what()));
    // }


    // BFCAD::Logger::log("Program end");

    return app.exec();
}