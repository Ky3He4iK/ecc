//
// Created by ky3he4ik on 12/12/2019.
//

#include "LongIntWidget.h"

LongIntWidget::LongIntWidget(const QString &_label, int input_len, bool is_for_key, QWidget *) {
    layout = new QHBoxLayout();

    label = new QLabel(_label);
    layout->addWidget(label);

    line = new QLineEdit();
    QString input_mask;
    if (is_for_key)
        input_mask = "0H ";
    for (int i = 0; i < input_len; i+= 16)
        input_mask += "HHHH ";
    line->setInputMask(input_mask);
    line->setText(input_mask.replace('H', '0'));

    layout->addWidget(line);

    setLayout(layout);

    connect(line, &QLineEdit::textChanged, this, &LongIntWidget::contentChangedSlot);
}

QString LongIntWidget::getContents() const {
    return line->text();
}

void LongIntWidget::setContents(const QString &new_content) {
    line->setText(new_content);
}

void LongIntWidget::setEnabled(bool enabled) {
    line->setEnabled(enabled);
}

void LongIntWidget::contentChangedSlot(const QString &) {
    emit contentChangedSignal();
}
