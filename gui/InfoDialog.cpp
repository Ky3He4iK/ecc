//
// Created by ky3he4ik on 18/12/2019.
//

#include "InfoDialog.h"

#include <QVBoxLayout>

InfoDialog::InfoDialog(const QString &_text, QWidget *) {

    text = new QLabel(text);
    ok = new QPushButton();

    auto layout = new QVBoxLayout;
    layout->addWidget(text);
    layout->addWidget(ok);

    connect(ok, &QPushButton::pressed, this, &InfoDialog::ok_pressed);
}

void InfoDialog::ok_pressed() {
    accept();
}
