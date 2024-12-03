#pragma once

#include <QMainWindow>

namespace BFCAD::UI {

    class BfcGui : public QMainWindow {
    Q_OBJECT
    
    public:

        BfcGui(QWidget *parent = 0);
    
    Q_SIGNALS:
        void save_file();

    };

}