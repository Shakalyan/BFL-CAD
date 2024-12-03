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
    QPixmap openpix("media/open.png");
    QPixmap savepix("media/save.png");

    QAction *compile_act = new QAction(compilepix, "Compile", this);
    QAction *open_file_act = new QAction(openpix, "Open file", this);
    QAction *save_act = new QAction(savepix, "Save file", this);
    
    QToolBar *toolbar = addToolBar("main toolbar");
    toolbar->addAction(compile_act);
    toolbar->addAction(open_file_act);
    toolbar->addAction(save_act);

    statusBar()->showMessage("Ready");

    FrameContainer *frame_container = new FrameContainer(this);
    setCentralWidget(frame_container);
}