#pragma once

#include <QFrame>
#include <QWidget>
#include <QString>

namespace BFCAD::UI 
{

    class SidePanel : public QFrame 
    {
        Q_OBJECT

        public:
            SidePanel(QWidget *parent = 0);
        
        Q_SIGNALS:
            void itemSelected(QString const& filePath);

    };

}