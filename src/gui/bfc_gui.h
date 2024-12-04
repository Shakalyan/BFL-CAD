#pragma once

#include "bf.h"

#include <QMainWindow>

namespace BFCAD::UI {

    class BfcGui : public QMainWindow {
    Q_OBJECT
    
    public:

        BfcGui(QWidget *parent = 0);
    
    Q_SIGNALS:
        void save_file();
    
    public Q_SLOTS:
        void onShowResultWindow(BooleanFunction *initial_bf, BooleanFunction *optimized_bf);

    };

}