#include "frames/side_panel.h"

#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QTreeView>

#include <iostream>

BFCAD::UI::SidePanel::SidePanel(QWidget *parent) : QFrame(parent)
{
    QFileSystemModel *model = new QFileSystemModel();
    model->setRootPath(QDir::currentPath());
    QTreeView *tree_view = new QTreeView(this);
    tree_view->setModel(model);
    tree_view->setRootIndex(model->index(QDir::currentPath()));
    tree_view->setHeaderHidden(true);
    for (int i = 1; i < model->columnCount(); ++i)
        tree_view->setColumnHidden(i, true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tree_view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    
    connect(tree_view, &QTreeView::clicked, this, [this, model](QModelIndex const& index) {
        std::cout << "click!" << std::endl;
        Q_EMIT itemSelected(model->filePath(index));
    });
}