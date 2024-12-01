#pragma once

#include <QFrame>
#include <QWidget>
#include <QTextEdit>

namespace BFCAD::UI {

    class Editor : public QFrame
    {
        Q_OBJECT

        private:
            QTextEdit *text_field;

        public:
            Editor(QWidget *parent = 0);
        
        public slots:
            void readFile(QString const& filePath);

    };

}