//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_RESULTHOLDER_H
#define ECC_RESULTHOLDER_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QtWidgets/QLabel>
#include <QGridLayout>
#include <QString>

class ResultHolder : public QWidget {
Q_OBJECT

private: //todo: signing
    QTextEdit *clearText;
    QTextEdit *cypherText;
    QTextEdit *base64Cypher;
    QTextEdit *signText;

    QLabel *clearTextLabel;
    QLabel *cypherTextLabel;
    QLabel *base64CypherLabel;
    QLabel *signTextLabel;

    QPushButton *encodeButton;
    QPushButton *decodeButton;

    QGridLayout *layout;

    // 0 - standby
    // 1 - waiting for encoding
    // 2 - waiting for decoding
    int state = 0;
    bool textChangedLock = false;

    void setActive(bool active);

public:
    explicit ResultHolder(QWidget *parent = Q_NULLPTR);

signals:

    void encodeSignal(const QString &cleartext);

    void decodeSignal(const QString &cyphertext, const QString &sign);

private slots:

    void encodeButtonSlot();

    void decodeButtonSlot();

    void base64TextChangedSlot();

    void cypherTextChangedSlot();

public slots:

    void encodingFinishedSlot(const QString &res, const QString &sign);

    void decodingFinishedSlot(const QString &res, bool sign_correct);

};


#endif //ECC_RESULTHOLDER_H
