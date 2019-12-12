//
// Created by ky3he4ik on 12/12/2019.
//

#include "ResultHolder.h"

ResultHolder::ResultHolder(QWidget *parent) {
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

    encodeButton = new QPushButton("Encode");
    decodeButton = new QPushButton("Decode");
    layout->addWidget(encodeButton, 3, 0);
    layout->addWidget(decodeButton, 3, 1);

    setLayout(layout);

    connect(encodeButton, &QPushButton::pressed, this, &ResultHolder::encodeButtonSlot);
    connect(decodeButton, &QPushButton::pressed, this, &ResultHolder::decodeButtonSlot);

    connect(cypherText, &QTextEdit::textChanged, this, &ResultHolder::cypherTextChangedSlot);
    connect(base64Cypher, &QTextEdit::textChanged, this, &ResultHolder::base64TextChangedSlot);
}

void ResultHolder::encodeButtonSlot() {
    encodeButton->setEnabled(false);
    decodeButton->setEnabled(false);
    encodeButton->setChecked(true);
    state = 1;
    emit encodeSignal(clearText->toPlainText());
}

void ResultHolder::decodeButtonSlot() {
    encodeButton->setEnabled(false);
    decodeButton->setEnabled(false);
    decodeButton->setChecked(true);
    state = 2;
    emit decodeSignal(cypherText->toPlainText());
}

void ResultHolder::calculationFinished(const QString &res) {
    switch (state) {
        case 1:
            cypherText->setPlainText(res);
            cypherTextChangedSlot();
            break;
        case 2:
            clearText->setPlainText(res);
            break;
        default:
            return;
    }
    state = 0;
    encodeButton->setEnabled(true);
    decodeButton->setEnabled(true);
    encodeButton->setChecked(false);
    decodeButton->setChecked(false);
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
