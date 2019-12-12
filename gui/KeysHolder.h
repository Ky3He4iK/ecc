//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_KEYSHOLDER_H
#define ECC_KEYSHOLDER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

#include "LongIntWidget.h"

class KeysHolder : public QWidget {
Q_OBJECT

private:
    LongIntWidget *private_key;
    LongIntWidget *my_public_key;
    LongIntWidget *other_public_key;
    LongIntWidget *shared_secret;

    QPushButton *load_keypair;
    QPushButton *load_other_public_key;
    QPushButton *generate_keypair;
    QPushButton *save_keypair;

    QGridLayout *layout;

    // 0 - standby
    // 1 - keypair generation requested
    // 2 - public key by private requested
    // 3 - shared secret by keys requested
    int state = 0;

    void requestShared();

public:
    explicit KeysHolder(QWidget *parent = Q_NULLPTR);

    void setEnabled(bool enabled);

private slots:

    void loadKeypairSlot();

    void loadOtherKeySlot();

    void generateKeypairSlot();

    void saveKeypairSlot();

    void privateKeyChangedSlot();

public slots:

    void keypairGenerated(const QString &new_private_key, const QString &public_key);

    void sharedGenerated(const QString &shared);

    void publicByPrivateGenerated(const QString &public_key);

    void emitKeys();

signals:

    void keys(const QString &private_key, const QString &public_key, const QString &other_public_key,
              const QString &shared_secred);

    void requestKeyGenerating();

    void requestPublicByPrivate(const QString &private_key);

    void requestSharedSecretGenerate(const QString &private_key, const QString &public_key,
                                     const QString &other_public_key);

};


#endif //ECC_KEYSHOLDER_H
