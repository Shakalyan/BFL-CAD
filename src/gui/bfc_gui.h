#pragma once

#include <QMainWindow>

namespace BFCAD::UI {

    class BfcGui : public QMainWindow {
    Q_OBJECT
    
    public:

        BfcGui(QWidget *parent = 0);
    
    signals:
        void save_file();

    };

}