//
// Created by ky3he4ik on 12/12/2019.
//

#include "KeysHolder.h"

KeysHolder::KeysHolder(QWidget *) {
    layout = new QGridLayout();

    private_key = new LongIntWidget("Your private key");
    layout->addWidget(private_key, 0, 0);

    my_public_key = new LongIntWidget("Your public key");
    my_public_key->setEnabled(false);
    layout->addWidget(my_public_key, 1, 0);

    other_public_key = new LongIntWidget("Companion's public key");
    layout->addWidget(other_public_key, 2, 0);

    shared_secret = new LongIntWidget("Shared secret key");
    shared_secret->setEnabled(false);
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

    connect(private_key, &LongIntWidget::contentChanged, this, &KeysHolder::privateKeyChangedSlot);

    generateKeypairSlot();
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
    //todo: timeout before requesting public key
    if (state != 0)
        return;
    state = 2;
    setEnabled(false);
    emit requestPublicByPrivate(private_key->getContents());
}

void KeysHolder::keypairGenerated(const QString &new_private_key, const QString &public_key) {
    if (state != 1)
        return;
    private_key->setContents(new_private_key);
    my_public_key->setContents(public_key);
    setEnabled(true);
    state = 0;
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
