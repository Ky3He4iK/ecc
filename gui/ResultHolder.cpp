//
// Created by ky3he4ik on 12/12/2019.
//

#include "ResultHolder.h"

ResultHolder::ResultHolder(QWidget *) {
    layout = new QGridLayout();

    clearText = new QTextEdit();
    clearTextLabel = new QLabel("Clear text:");
    clearText->setPlaceholderText("Insert message you want to encode here");
    layout->addWidget(clearText, 0, 1);
    layout->addWidget(clearTextLabel, 0, 0);

    cypherText = new QTextEdit();
    cypherTextLabel = new QLabel("Cypher text:");
    cypherText->setPlaceholderText("Insert message you want to decode here");
    layout->addWidget(cypherText, 1, 1);
    layout->addWidget(cypherTextLabel, 1, 0);

    base64Cypher = new QTextEdit();
    base64CypherLabel = new QLabel("Cypher text:\n(base64)");
    base64Cypher->setPlaceholderText("Insert encrypted message in base64 here");
    layout->addWidget(base64Cypher, 2, 1);
    layout->addWidget(base64CypherLabel, 2, 0);

    signText = new QTextEdit();
    signTextLabel = new QLabel("sign:");
    signText->setPlaceholderText("Sign of encrypted message");
    layout->addWidget(signText, 3, 1);
    layout->addWidget(signTextLabel, 3, 0);

    encodeButton = new QPushButton("Encode");
    decodeButton = new QPushButton("Decode");
    layout->addWidget(encodeButton, 4, 0);
    layout->addWidget(decodeButton, 4, 1);

    setLayout(layout);

    connect(encodeButton, &QPushButton::pressed, this, &ResultHolder::encodeButtonSlot);
    connect(decodeButton, &QPushButton::pressed, this, &ResultHolder::decodeButtonSlot);

    connect(cypherText, &QTextEdit::textChanged, this, &ResultHolder::cypherTextChangedSlot);
    connect(base64Cypher, &QTextEdit::textChanged, this, &ResultHolder::base64TextChangedSlot);
}

void ResultHolder::encodeButtonSlot() {
    state = 1;
    encodeButton->setChecked(true);
    setActive(false);
    emit encodeSignal(clearText->toPlainText());
}

void ResultHolder::decodeButtonSlot() {
    state = 2;
    decodeButton->setChecked(true);
    setActive(false);
    emit decodeSignal(cypherText->toPlainText(), signText->toPlainText());
}

void ResultHolder::encodingFinishedSlot(const QString &res, const QString &sign) {
    if (state != 1)
        return;
    cypherText->setPlainText(res);
    cypherTextChangedSlot();
    setActive(true);
    encodeButton->setChecked(false);
    decodeButton->setChecked(false);
    state = 0;
}

void ResultHolder::decodingFinishedSlot(const QString &res, bool sign_correct) {
    //todo: use sign_correct
    if (state != 2)
        return;
    clearText->setPlainText(res);
    setActive(true);
    encodeButton->setChecked(false);
    decodeButton->setChecked(false);
    state = 0;
}

void ResultHolder::cypherTextChangedSlot() {
    if (textChangedLock)
        return;
    textChangedLock = true;
    base64Cypher->setPlainText(cypherText->toPlainText().toUtf8().toBase64());
    textChangedLock = false;
}

void ResultHolder::base64TextChangedSlot() {
    if (textChangedLock)
        return;
    textChangedLock = true;
    cypherText->setPlainText(QString(QByteArray::fromBase64(base64Cypher->toPlainText().toUtf8())));
    textChangedLock = false;
}

void ResultHolder::setActive(bool active) {
    clearText->setEnabled(active);
    cypherText->setEnabled(active);
    base64Cypher->setEnabled(active);
    signText->setEnabled(active);
    encodeButton->setEnabled(active);
    decodeButton->setEnabled(active);
}
