//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_CURVEWIDGET_H
#define ECC_CURVEWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QVector>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

#include "LongIntWidget.h"

class CurveWidget : public QWidget {
Q_OBJECT

private:
    LongIntWidget *a;
    LongIntWidget *b;
    LongIntWidget *p;
    LongIntWidget *base_point;
    LongIntWidget *order;

    QGroupBox *curve_selector;
    QVector<QPushButton* > curve_options;

    QPushButton *load_curve;
    QPushButton *save_curve;

    QGridLayout *layout;
    QVBoxLayout *selector_layout;

public:
    CurveWidget(QWidget *parent = Q_NULLPTR);
};


#endif //ECC_CURVEWIDGET_H
