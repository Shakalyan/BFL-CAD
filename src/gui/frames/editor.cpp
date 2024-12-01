#include "frames/editor.h"

#include <QVBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTabBar>

BFCAD::UI::FileTab::FileTab(QString const& file_path)
{
    this->file_path = file_path;
    this->file_name = QFileInfo(file_path).fileName();
}

bool BFCAD::UI::FileTab::operator==(FileTab const& file_tab) const
{
    return this->file_path == file_tab.file_path;
}

BFCAD::UI::Editor::Editor(QWidget *parent) : QFrame(parent)
{
    this->text_field = new QTextEdit(this);
    this->tabs = new QTabWidget(this);

    this->tabs->setFixedHeight(25);
    this->tabs->setTabsClosable(true);
    this->tabs->setUsesScrollButtons(false);
    tabs->setStyleSheet(R"(
        QTabBar::tab {
            min-width: 50px; /* Минимальная ширина */
            max-width: 150px; /* Максимальная ширина */
        }
    )");
    this->tabs->tabBar()->setElideMode(Qt::ElideRight);
    

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->tabs);
    layout->addWidget(this->text_field);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setStretchFactor(this->tabs, 0);
    layout->setStretchFactor(this->text_field, 1);
    setLayout(layout);

    connect(this->tabs, &QTabWidget::tabCloseRequested, this, &Editor::closeFile);
}

void BFCAD::UI::Editor::openFile(QString const& file_path)
{
    QFile file(file_path);
    QFileInfo file_info(file_path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString file_data(in.readAll());
    file.close();
    
    FileTab *current_tab = nullptr;
    for (int i = 0; i < this->tabs->count(); ++i) {
        FileTab *file_tab = static_cast<FileTab*>(this->tabs->widget(i));
        if (file_tab->file_path == file_path) {
            current_tab = file_tab;
            break;
        }
    }
    if (!current_tab) {
        current_tab = new FileTab(file_path);
        this->tabs->addTab(current_tab, current_tab->file_name);
    }
    this->tabs->setCurrentWidget(current_tab);

    this->text_field->setText(file_data);
}

void BFCAD::UI::Editor::closeFile(int tab_index)
{
    this->tabs->removeTab(tab_index);

    int tabs_count = this->tabs->count();
    if (tabs_count) {
        emit openFile(static_cast<FileTab*>(this->tabs->widget(tabs_count-1))->file_path);
    }
    else {
        this->text_field->setText("");
    }    
}