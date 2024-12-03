#pragma once

#include "frames/editor.h"
#include "frames/side_panel.h"
#include "frames/output_panel.h"

#include <QWidget>

namespace BFCAD::UI
{

    class FrameContainer : public QWidget
    {
        public:
            Editor *editor;
            SidePanel *side_panel;
            OutputPanel *output_panel;
            
        public:
            FrameContainer(QWidget *parent = 0);
        
    };

}