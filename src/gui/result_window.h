#pragma once

#include "bf.h"

#include <QMainWindow>
#include <QWidget>
#include <QFrame>

namespace BFCAD::UI 
{

    class BFInfoFrame : public QFrame
    {

    public:
        BFInfoFrame(std::unique_ptr<BooleanFunction> bf, QWidget *parent = 0);

    };

    class ResultWindow : public QMainWindow
    {

    public:

        ResultWindow(std::unique_ptr<BooleanFunction> initial_bf, std::unique_ptr<BooleanFunction> optimized_bf, QWidget *parent = 0);

    };

}