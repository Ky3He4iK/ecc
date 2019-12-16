//
// Created by ky3he4ik on 12/12/2019.
//

#include "KeysHolder.h"

KeysHolder::KeysHolder(QWidget *) {
    layout = new QGridLayout();

    int int_len = 256;
    private_key = new LongIntWidget("Your private key", false);
    private_key->setInputLen(int_len);
    layout->addWidget(private_key, 0, 0);

    my_public_key = new LongIntWidget("Your public key", true);
    my_public_key->setEnabled(false);
    my_public_key->setInputLen(int_len);
    layout->addWidget(my_public_key, 1, 0);

    other_public_key = new LongIntWidget("Companion's public key", true);
    other_public_key->setInputLen(int_len);
    layout->addWidget(other_public_key, 2, 0);

    shared_secret = new LongIntWidget("Shared secret key", false);
    shared_secret->setEnabled(false);
    shared_secret->setInputLen(int_len);
    layout->addWidget(shared_secret, 3, 0);

    generate_keypair = new QPushButton("Generate new keypair");
    layout->addWidget(generate_keypair, 0, 1);

    load_keypair = new QPushButton("Load keypair from file");
    layout->addWidget(load_keypair, 1, 1);

    save_keypair = new QPushButton("Save current keypair to file");
    layout->addWidget(save_keypair, 2, 1);

    load_other_public_key = new QPushButton("Load other public key");
    layout->addWidget(load_other_public_key, 3, 1);

    setLayout(layout);

    connect(generate_keypair, &QPushButton::pressed, this, &KeysHolder::generateKeypairSlot);
    connect(load_keypair, &QPushButton::pressed, this, &KeysHolder::loadKeypairSlot);
    connect(save_keypair, &QPushButton::pressed, this, &KeysHolder::saveKeypairSlot);
    connect(load_other_public_key, &QPushButton::pressed, this, &KeysHolder::loadOtherKeySlot);

    connect(private_key, &LongIntWidget::contentChangedSignal, this, &KeysHolder::privateKeyChangedSlot);

//    generateKeypairSlot();
}

void KeysHolder::loadKeypairSlot() {
    //todo
}

void KeysHolder::loadOtherKeySlot() {
    //todo
}

void KeysHolder::generateKeypairSlot() {
    state = 1;
    setEnabled(false);
    emit requestKeyGenerating();
}

void KeysHolder::saveKeypairSlot() {
    //todo
}

void KeysHolder::privateKeyChangedSlot() {
    if (state != 0)
        return;
    state = 2;
    setEnabled(false);
    emit requestPublicByPrivate(private_key->getContents());
}

void KeysHolder::keypairGenerated(const QString &new_private_key, const QString &public_key) {
//    if (state != 1)
//        return;
    private_key->setContents(new_private_key);
    my_public_key->setContents(public_key);
    setEnabled(true);
    state = 0;
    if (!other_public_key->getContents().isEmpty())
        requestShared();
}

void KeysHolder::publicByPrivateGenerated(const QString &public_key) {
    if (state != 2)
        return;
    my_public_key->setContents(public_key);
    setEnabled(true);
    state = 0;

}

void KeysHolder::sharedGenerated(const QString &shared) {
    if (state != 3)
        return;
    shared_secret->setContents(shared);
    setEnabled(true);
    state = 0;
}

void KeysHolder::emitKeys() {
    emit keys(private_key->getContents(), my_public_key->getContents(), other_public_key->getContents(),
              shared_secret->getContents());
}

void KeysHolder::setEnabled(bool enabled) {
    generate_keypair->setEnabled(enabled);
    load_keypair->setEnabled(enabled);
    save_keypair->setEnabled(enabled);
    load_other_public_key->setEnabled(enabled);

    private_key->setEnabled(enabled);
    other_public_key->setEnabled(enabled);
}

void KeysHolder::requestShared() {
    state = 3;
    setEnabled(false);
    emit requestSharedSecretGenerate(private_key->getContents(), my_public_key->getContents(),
                                     other_public_key->getContents());
}

void KeysHolder::changeIntLen(int int_len) {
    private_key->setInputLen(int_len);
    my_public_key->setInputLen(int_len);
    other_public_key->setInputLen(int_len);
    shared_secret->setInputLen(int_len);
}
