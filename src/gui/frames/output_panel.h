#pragma once

#include <QFrame>
#include <QWidget>
#include <QTextEdit>

namespace BFCAD::UI
{

    class OutputPanel : public QFrame
    {
        Q_OBJECT

        private:
            QTextEdit *text_field;

        public:
            OutputPanel(QWidget *parent = 0);
        
        public slots:
            void displayText(QString const& text);
    };

}