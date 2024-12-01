#include "frames/output_panel.h"

#include <QVBoxLayout>

BFCAD::UI::OutputPanel::OutputPanel(QWidget *parent) : QFrame(parent)
{
    this->text_field = new QTextEdit(this);
    this->text_field->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->text_field);

    setLayout(layout);
}

void BFCAD::UI::OutputPanel::displayText(QString const& text)
{
    this->text_field->setText(text);
}