#include "bfc_gui.h"

#include "bfc_frame_container.h"
#include "result_window.h"//-----------------------
#include "logger.h"

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
    QPixmap optimizepix("media/play-button.png");
    QPixmap openpix("media/open.png");
    QPixmap savepix("media/save.png");

    QAction *optimize_act = new QAction(optimizepix, "Compile", this);
    optimize_act->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));

    QAction *open_file_act = new QAction(openpix, "Open file", this);
    open_file_act->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

    QAction *save_act = new QAction(savepix, "Save file", this);
    save_act->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    
    QToolBar *toolbar = addToolBar("main toolbar");
    toolbar->addAction(optimize_act);
    toolbar->addAction(open_file_act);
    toolbar->addAction(save_act);

    statusBar()->showMessage("Ready");

    FrameContainer *frame_container = new FrameContainer(this);
    setCentralWidget(frame_container);

    connect(open_file_act, &QAction::triggered, frame_container->editor, &Editor::searchFile);
    connect(save_act, &QAction::triggered, frame_container->editor, &Editor::saveFile);
    connect(optimize_act, &QAction::triggered, frame_container->editor, &Editor::optimize);
    connect(frame_container->editor, &Editor::showResultWindow, this, &BfcGui::onShowResultWindow);
}

void BFCAD::UI::BfcGui::onShowResultWindow(BooleanFunction *ibf, BooleanFunction *obf)
{
    std::unique_ptr<BooleanFunction> initial_bf(ibf);
    std::unique_ptr<BooleanFunction> optimized_bf(obf);
    ResultWindow *rw = new ResultWindow(std::move(initial_bf), std::move(optimized_bf), this);
    rw->adjustSize();
    rw->show();
}
