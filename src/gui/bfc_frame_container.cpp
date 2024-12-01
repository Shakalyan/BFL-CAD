#include "bfc_frame_container.h"

#include "frames/editor.h"
#include "frames/side_panel.h"
#include "frames/output_panel.h"

#include <QFrame>
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>

BFCAD::UI::FrameContainer::FrameContainer(QWidget *parent) : QWidget(parent)
{
    Editor *editor = new Editor(this);
    OutputPanel *output_panel = new OutputPanel(this);
    SidePanel *side_panel = new SidePanel(this);

    side_panel->setFrameShape(QFrame::StyledPanel);
    editor->setFrameShape(QFrame::StyledPanel);
    output_panel->setFrameShape(QFrame::StyledPanel);

    QSplitter *horizonal_splitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *vertical_splitter = new QSplitter(Qt::Vertical, this);
    vertical_splitter->addWidget(editor);
    vertical_splitter->addWidget(output_panel);
    horizonal_splitter->addWidget(side_panel);
    horizonal_splitter->addWidget(vertical_splitter);

    QList<int> sizes({200, 200});
    vertical_splitter->setSizes(sizes);
    horizonal_splitter->setSizes(sizes);

    vertical_splitter->setStretchFactor(0, 1);
    vertical_splitter->setStretchFactor(1, 0);
    horizonal_splitter->setStretchFactor(0, 0);
    horizonal_splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(horizonal_splitter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(side_panel, &SidePanel::itemSelected, output_panel, &OutputPanel::displayText);
    connect(side_panel, &SidePanel::itemSelected, editor, &Editor::openFile);
}