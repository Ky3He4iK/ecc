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
#include <QLabel>

#include "LongIntWidget.h"
#include "../curve/ECC.h"

class CurveWidget : public QWidget {
Q_OBJECT

private:
    LongIntWidget *a;
    LongIntWidget *b;
    LongIntWidget *p;
    LongIntWidget *base_point;
    LongIntWidget *order;

    QGroupBox *curve_selector;
    QVector<QPushButton *> curve_options;

    QPushButton *load_curve;
    QPushButton *save_curve;
    QPushButton *len_selecter;

    QGridLayout *layout;
    QVBoxLayout *selector_layout;

    bool lock = false;

    int state = 0;
    int last_curve = -1;

    ECC ecc;

    QString temp;
    QString temp2;

    void toCustom();

public:
    explicit CurveWidget(QWidget *parent = Q_NULLPTR);

private slots:

    void longIntChanged(int new_int);

    void loadCurveSlot();

    void saveCurveSlot();

    void curveSelectedSlot();

public slots:

    void encodeSlot(const QString &cleartext);

    void decodeSlot(const QString &cyphertext, const QString &sign);


    void receiveKeysSlot(const QString &private_key, const QString &public_key, const QString &other_public_key,
                         const QString &shared_secret);

    void requestKeyGeneratingSlot();

    void requestPublicByPrivateSlot(const QString &private_key);

    void requestSharedSecretGenerate(const QString &private_key, const QString &public_key,
                                     const QString &other_public_key);

signals:

    void encodingFinishedSignal(const QString &res, const QString &sign);

    void decodingFinishedSignal(const QString &res, bool sign_correct);


    void keypairGeneratedSignal(const QString &new_private_key, const QString &public_key);

    void sharedGeneratedSignal(const QString &shared_key);

    void publicByPrivateGeneratedSignal(const QString &public_key);

    void requestKeysSignal();

    void changeIntLenSignal(int new_len);
};


#endif //ECC_CURVEWIDGET_H
