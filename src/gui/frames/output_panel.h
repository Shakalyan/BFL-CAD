#pragma once

#include <QFrame>
#include <QWidget>
#include <QTextEdit>
#include <streambuf>
#include <ostream>

namespace BFCAD::UI
{

    class OutputPanelBuffer : public std::streambuf
    {
    private:
        QTextEdit *text_field;
        QString text_buffer;

    private:
        void flush();

    public:
        OutputPanelBuffer(QTextEdit *text_field);
    
    protected:

        int overflow(int c) override;

        std::streamsize xsputn(const char *s, std::streamsize count) override;

    };

    class OutputPanelStream : public std::ostream
    {
    private:
        OutputPanelBuffer buffer;

    public:

        OutputPanelStream(QTextEdit *text_field);

    };

    class OutputPanel : public QFrame
    {
        Q_OBJECT

        private:
            QTextEdit *text_field;
            OutputPanelStream *ostream;

        public:
            OutputPanel(QWidget *parent = 0);
            ~OutputPanel();
        
        public Q_SLOTS:
            void onTextChanged();
            void displayText(QString const& text);
    };

}