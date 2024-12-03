#include "frames/editor.h"

#include <QVBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTabBar>
#include <QFileDialog>
#include <iostream>

BFCAD::UI::FileTab::FileTab(QString const& file_path)
{
    this->file_path = file_path;
    this->file_name = QFileInfo(file_path).fileName();
    this->unsaved = false;

    QFile file(file_path);
    QFileInfo file_info(file_path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString file_data(in.readAll());
    file.close();

    this->file_content = std::move(file_data);
}

bool BFCAD::UI::FileTab::operator==(FileTab const& file_tab) const
{
    return this->file_path == file_tab.file_path;
}

BFCAD::UI::Editor::Editor(QWidget *parent) : QFrame(parent)
{
    this->text_field = new QTextEdit(this);
    this->tabs = new QTabWidget(this);
    this->current_tab = nullptr;

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
    connect(this->tabs, &QTabWidget::currentChanged, this, &Editor::openTab);
    connect(this->text_field, &QTextEdit::textChanged, this, &Editor::onTextChanged);
}

void BFCAD::UI::Editor::openFile(QString const& file_path)
{    
    FileTab *tab = nullptr;
    for (int i = 0; i < this->tabs->count(); ++i) {
        FileTab *file_tab = static_cast<FileTab*>(this->tabs->widget(i));
        if (file_tab->file_path == file_path) {
            tab = file_tab;
            break;
        }
    }
    if (!tab) {
        tab = new FileTab(file_path);
        this->tabs->addTab(tab, tab->file_name);
    }
    this->tabs->setCurrentWidget(tab);
}

void BFCAD::UI::Editor::openTab(int tab_index)
{
    int tabs_count = this->tabs->count();
    if (tabs_count == 0 || tab_index < 0 || tab_index >= tabs_count) {
        return;
    }

    FileTab *tab = static_cast<FileTab*>(this->tabs->widget(tab_index));
    this->tabs->setCurrentWidget(tab);
    
    if (this->current_tab) {
        this->current_tab->file_content = this->text_field->toPlainText();
    }
    this->current_tab = tab;

    this->set_text_blocked(tab->file_content);
}

void BFCAD::UI::Editor::closeFile(int tab_index)
{
    this->tabs->removeTab(tab_index);

    int tabs_count = this->tabs->count();
    if (tabs_count) {
        this->openFile(static_cast<FileTab*>(this->tabs->widget(tabs_count-1))->file_path);
    }
    else {
        this->current_tab = nullptr;
        this->set_text_blocked("");
    }    
}

void BFCAD::UI::Editor::searchFile()
{
    QString chosen_file_path = QFileDialog::getOpenFileName(this, "Choose file", QDir::currentPath(), "*.bfl");
    if (!chosen_file_path.isEmpty())
        this->openFile(chosen_file_path);
}

void BFCAD::UI::Editor::onTextChanged()
{
    if (this->current_tab) {
        int tab_index = this->tabs->indexOf(this->current_tab);
        this->tabs->setTabText(tab_index, "*" + this->current_tab->file_name);
        this->current_tab->unsaved = true;
    }
}

void BFCAD::UI::Editor::saveFile()
{
    if (this->current_tab && this->current_tab->unsaved) {
        std::cout << "save" << std::endl;
        this->current_tab->file_content = this->text_field->toPlainText();

        QFile file(this->current_tab->file_path);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << this->current_tab->file_content;
        out.flush();
        file.close();

        int tab_index = this->tabs->indexOf(this->current_tab);
        this->tabs->setTabText(tab_index, this->current_tab->file_name);
        this->current_tab->unsaved = false;
    }
}

void BFCAD::UI::Editor::set_text_blocked(QString const& text)
{
    bool blocked = this->text_field->signalsBlocked();
    this->text_field->blockSignals(true);
    this->text_field->setText(text);
    this->text_field->blockSignals(blocked);
}
