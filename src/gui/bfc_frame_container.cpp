#include "bfc_frame_container.h"

#include <QFrame>
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>

BFCAD::UI::FrameContainer::FrameContainer(QWidget *parent) : QWidget(parent)
{
    this->editor = new Editor(this);
    this->output_panel = new OutputPanel(this);
    this->side_panel = new SidePanel(this);

    this->side_panel->setFrameShape(QFrame::StyledPanel);
    this->editor->setFrameShape(QFrame::StyledPanel);
    this->output_panel->setFrameShape(QFrame::StyledPanel);

    QSplitter *horizonal_splitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *vertical_splitter = new QSplitter(Qt::Vertical, this);
    vertical_splitter->addWidget(this->editor);
    vertical_splitter->addWidget(this->output_panel);
    horizonal_splitter->addWidget(this->side_panel);
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

    connect(this->side_panel, &SidePanel::itemSelected, this->output_panel, &OutputPanel::displayText);
    connect(this->side_panel, &SidePanel::itemSelected, this->editor, &Editor::openFile);
}