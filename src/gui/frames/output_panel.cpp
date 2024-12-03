#include "frames/output_panel.h"

#include "logger.h"
#include <QVBoxLayout>
#include <iostream>

BFCAD::UI::OutputPanelBuffer::OutputPanelBuffer(QTextEdit *text_field)
{
    this->text_field = text_field;
}

void BFCAD::UI::OutputPanelBuffer::flush()
{
    this->text_buffer.chop(1);
    this->text_field->append(this->text_buffer);
    this->text_buffer.clear();
}

int BFCAD::UI::OutputPanelBuffer::overflow(int c)
{
    if (c != EOF) {
        this->text_buffer.append((char)c);
        this->flush();
    }
    return c;
}

std::streamsize BFCAD::UI::OutputPanelBuffer::xsputn(const char *s, std::streamsize count)
{
    this->text_buffer.append(QString::fromUtf8(s, count));
    if (this->text_buffer.endsWith('\n'))
        this->flush();
    return count;
}

BFCAD::UI::OutputPanelStream::OutputPanelStream(QTextEdit *text_field) : std::ostream(&buffer), buffer(text_field)
{
    
}

BFCAD::UI::OutputPanel::OutputPanel(QWidget *parent) : QFrame(parent)
{
    this->text_field = new QTextEdit(this);
    this->text_field->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->text_field);

    this->ostream = new OutputPanelStream(this->text_field);
    Logger::add_ostream(this->ostream);

    setLayout(layout);
}

BFCAD::UI::OutputPanel::~OutputPanel()
{
    delete this->ostream;
}

void BFCAD::UI::OutputPanel::displayText(QString const& text)
{
    this->text_field->setText(text);
}