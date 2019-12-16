//
// Created by ky3he4ik on 12/12/2019.
//

#include "CurveWidget.h"
#include "../curve/CurveManager.h"

#include <QMenu>

CurveWidget::CurveWidget(QWidget *) : ecc(ECC(Curve_parameters::curve_secp256k1())) {
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
        connect(*curve_options.rbegin(), &QPushButton::pressed, this, &CurveWidget::curveSelectedSlot);
    }

    curve_options[0]->setChecked(true);

    curve_options.push_back(new QPushButton("custom"));
    selector_layout->addWidget(*curve_options.rbegin());
    curve_selector = new QGroupBox("Choose curve:");

    curve_selector->setLayout(selector_layout);
    layout->addWidget(curve_selector, 0, 1, 2, 1);

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

    len_selecter = new QPushButton(QString::number(bit_len) + " bit");
    auto *len_menu = new QMenu();
    len_menu->addAction("192 bit");
    len_menu->addAction("224 bit");
    len_menu->addAction("256 bit");
    len_menu->addAction("384 bit");
    len_menu->addAction("521 bit");
    len_selecter->setMenu(len_menu);
    layout->addWidget(len_selecter, 2, 1);

    load_curve = new QPushButton("Load curve from file");
    layout->addWidget(load_curve, 3, 1);

    save_curve = new QPushButton("Save curve to file");
    layout->addWidget(save_curve, 4, 1);

    setLayout(layout);

    curveSelectedSlot();

    connect(load_curve, &QPushButton::pressed, this, &CurveWidget::loadCurveSlot);
    connect(save_curve, &QPushButton::pressed, this, &CurveWidget::saveCurveSlot);
    //todo: connect to longIntChanged
}

void CurveWidget::longIntChanged(int new_len) {
    if (!lock)
        toCustom();
    emit changeIntLenSignal(new_len);
}

void CurveWidget::loadCurveSlot() {
    //todo
}

void CurveWidget::saveCurveSlot() {
    //todo
}

void CurveWidget::curveSelectedSlot() {
    if ((*curve_options.rbegin())->isChecked()) {
        toCustom();
        return;
    }
    lock = true;
    for (int i = 0; i < curve_options.size(); i++)
        if (curve_options[i]->isChecked()) {
            if (i != last_curve) {
                last_curve = i;
                auto[param, len] = CurveManager::getInstance().getCurves(curve_options[i]->text().toStdString());
                ecc = ECC(param);
                longIntChanged(len);

                a->setContents(QString::fromStdString(param.curve_a.to_string()));
                b->setContents(QString::fromStdString(param.curve_b.to_string()));
                p->setContents(QString::fromStdString(param.curve_p.to_string()));
                base_point->setContents(QString::fromStdString(param.base_point.to_string()));
                order->setContents(QString::fromStdString(param.curve_order.to_string()));
            }
            return;
        }
    lock = false;
}

void CurveWidget::encodeSlot(const QString &cleartext) {
    if (state != 0)
        return;
    state = 1;
    temp = cleartext;
    emit requestKeysSignal();
}

void CurveWidget::decodeSlot(const QString &cyphertext, const QString &sign) {
    if (state != 0)
        return;
    state = 2;
    temp = cyphertext;
    temp2 = sign;
    emit requestKeysSignal();
}


void CurveWidget::receiveKeysSlot(const QString &private_key, const QString &public_key,
                                  const QString &other_public_key, const QString &shared_secret) {
    if (state == 1 || state == 2) {
        ecc.set_keys(LongInt(private_key.toStdString(), 16), Point(ecc.get_curve(), private_key.toStdString()));
        ecc.set_shared_secret(Point(ecc.get_curve(), other_public_key.toStdString()));
    }
    if (state == 1) {
        auto std_str = temp.toStdString();
        auto res = ecc.encode(std_str);
        auto[f, s] = ecc.sign_msg(std_str);
        emit encodingFinishedSignal(QString::fromStdString(res),
                                    QString::fromStdString(f.to_string(16) + ';' + s.to_string(16)));
        state = 0;
    } else if (state == 2) {
        auto std_str = temp.toStdString();
        auto res = ecc.decode(std_str);
        auto sign = Sign();
        auto list = temp2.split(';');
        if (list.size() == 2)
            sign = std::make_pair(LongInt(list[0].toStdString(), 16), LongInt(list[1].toStdString(), 16));

        bool ver = ECC::verify_msg(std_str, sign, ecc.get_public_key(), ecc.get_parameters());
        emit decodingFinishedSignal(QString::fromStdString(res), ver);
        state = 0;
    }
}

void CurveWidget::requestKeyGeneratingSlot() {
    emit keypairGeneratedSignal(QString::fromStdString(ecc.get_private_key().to_string(16)),
                                QString::fromStdString(ecc.get_public_key().to_string()));
}

void CurveWidget::requestPublicByPrivateSlot(const QString &private_key) {
    emit publicByPrivateGeneratedSignal(
            QString::fromStdString(ecc.generatePublic(LongInt(private_key.toStdString(), 16)).to_string()));
}

void CurveWidget::requestSharedSecretGenerate(const QString &private_key, const QString &public_key,
                                              const QString &other_public_key) {
    ecc.set_keys(LongInt(private_key.toStdString(), 16), Point(ecc.get_curve(), private_key.toStdString()));
    emit sharedGeneratedSignal(QString::fromStdString(
            ecc.set_shared_secret(Point(ecc.get_curve(), other_public_key.toStdString())).to_string()));
}

void CurveWidget::toCustom() {
    last_curve = curve_options.size() - 1;
    curve_options[last_curve]->setChecked(true);
}
