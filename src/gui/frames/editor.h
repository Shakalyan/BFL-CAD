#pragma once

#include <QFrame>
#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>

namespace BFCAD::UI {

    class FileTab : public QWidget
    {
        public:
            QString file_path;
            QString file_name;
            QString file_content;

            FileTab(QString const& file_path);

            bool operator==(FileTab const& file_tab) const;
    };

    class Editor : public QFrame
    {
        Q_OBJECT

        private:
            QTextEdit *text_field;
            QTabWidget *tabs;
            FileTab *current_tab;

            void set_text_blocked(QString const& text);

        public:
            Editor(QWidget *parent = 0);
        
        public slots:
            void openFile(QString const& file_path);
            void openTab(int tab_index);
            void closeFile(int tab_index);
            void searchFile();
            void onTextChanged();

    };

}