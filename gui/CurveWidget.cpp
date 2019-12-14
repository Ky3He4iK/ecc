//
// Created by ky3he4ik on 12/12/2019.
//

#include "CurveWidget.h"
#include "../curve/CurveManager.h"

CurveWidget::CurveWidget(QWidget *) {
    layout = new QGridLayout();

    selector_layout = new QVBoxLayout();

    auto manager = CurveManager::getInstance();
    auto curve_names = manager.getCurveNames();
    int bit_len = 256;

    for (auto &curve_name: curve_names) {
        auto[curve, len] = manager.getCurves(curve_name);
        curve_options.push_back(new QPushButton(QString(curve_name.c_str())));
        selector_layout->addWidget(*curve_options.rbegin());
        if (curve_name == curve_names[0])
            bit_len = len;
    }

    curve_options[0]->setChecked(true);

    curve_options.push_back(new QPushButton("custom"));
    selector_layout->addWidget(*curve_options.rbegin());
    curve_selector = new QGroupBox("Choose curve:");

    curve_selector->setLayout(selector_layout);
    layout->addWidget(curve_selector, 0, 1, 3, 1);

    a = new LongIntWidget("a: ");
    a->setInputLen(bit_len);
    layout->addWidget(a, 0, 0);

    b = new LongIntWidget("b: ");
    b->setInputLen(bit_len);
    layout->addWidget(b, 1, 0);

    p = new LongIntWidget("p: ");
    p->setInputLen(bit_len);
    layout->addWidget(p, 2, 0);

    base_point = new LongIntWidget("base_point: ", true);
    base_point->setInputLen(bit_len);
    layout->addWidget(base_point, 3, 0);

    order = new LongIntWidget("curve order: ");
    order->setInputLen(bit_len);
    layout->addWidget(order, 4, 0);

    load_curve = new QPushButton("Load curve from file");
    layout->addWidget(load_curve, 3, 1);

    save_curve = new QPushButton("Save curve to file");
    layout->addWidget(save_curve, 4, 1);

    setLayout(layout);

    //todo: connect
}

void CurveWidget::longIntChanged() {

}

void CurveWidget::loadCurveSlot() {

}

void CurveWidget::saveCurveSlot() {

}

void CurveWidget::curveSelectedSlot(bool) {

}

void CurveWidget::encodeSlot(const QString &cleartext) {

}

void CurveWidget::decodeSlot(const QString &cyphertext, const QString &sign) {

}


void CurveWidget::receiveKeysSlot(const QString &private_key, const QString &public_key,
                                  const QString &other_public_key, const QString &shared_secred) {

}

void CurveWidget::requestKeyGeneratingSlot() {

}

void CurveWidget::requestPublicByPrivateSlot(const QString &private_key) {

}

void CurveWidget::requestSharedSecretGenerate(const QString &private_key, const QString &public_key,
                                              const QString &other_public_key) {

}