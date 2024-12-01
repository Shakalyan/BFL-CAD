#include "bfc_gui.h"

#include "bfc_frame_container.h"
#include <QFrame>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QStatusBar>

BFCAD::UI::BfcGui::BfcGui(QWidget *parent) : QMainWindow(parent)
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

    statusBar()->showMessage("Ready");

    FrameContainer *frame_container = new FrameContainer(this);
    setCentralWidget(frame_container);
}