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

    return app.exec();
}