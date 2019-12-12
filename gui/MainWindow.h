//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_MAINWINDOW_H
#define ECC_MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include "CurveWidget.h"
#include "KeysHolder.h"
#include "ResultHolder.h"

class MainWindow : public QWidget {
Q_OBJECT

private:
    CurveWidget *curveWidget;
    KeysHolder *keysHolder;
    ResultHolder *resultHolder;
    QGridLayout *layout;

public:

    MainWindow();

};


#endif //ECC_MAINWINDOW_H
