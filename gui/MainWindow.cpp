//
// Created by ky3he4ik on 12/12/2019.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    layout = new QGridLayout();

    curveWidget = new CurveWidget();
    layout->addWidget(curveWidget, 0, 0);

    keysHolder = new KeysHolder();
    layout->addWidget(keysHolder, 0, 1);

    resultHolder = new ResultHolder();
    layout->addWidget(resultHolder, 1, 0);

    setLayout(layout);
}
