#include "bfc_gui.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QStatusBar>

BFCAD::BfcGui::BfcGui(QWidget *parent) : QMainWindow(parent)
{
    // menu
    QAction *quit = new QAction("Quit", this);
    QMenu *file = menuBar()->addMenu("File");
    file->addAction(quit);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    // toolbar
    QPixmap compilepix("media/play-button.png");
    
    QToolBar *toolbar = addToolBar("main toolbar");
    toolbar->addAction(QIcon(compilepix), "Compile");
    
    QPushButton *button = new QPushButton("test", this);
    button->move(100, 100);

    statusBar()->showMessage("Ready");
}