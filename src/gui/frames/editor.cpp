#include "frames/editor.h"

#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

BFCAD::UI::Editor::Editor(QWidget *parent) : QFrame(parent)
{
    this->text_field = new QTextEdit(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->text_field);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void BFCAD::UI::Editor::readFile(QString const& filePath)
{
    QFile file(filePath);

    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString file_data(in.readAll());
    file.close();
    
    this->text_field->setText(file_data);
}