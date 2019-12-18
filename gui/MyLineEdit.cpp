//
// Created by ky3he4ik on 18/12/2019.
//

#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *) {
    connect(this, &MyLineEdit::editingFinished, this, &MyLineEdit::editedSlot);
}

void MyLineEdit::setValue(int bit_len, const QString &data, bool is_point) {
    if (is_point) {
        setText(data.isEmpty() ? QString(bit_len / 4 + 2, '0') : data);
        old = text();
        return;
    }
    auto s1 = QString();
    if (bit_len != -1) {
        auto d = bit_len - data.size() * 4;
        for (size_t i = 0; i < d; i += 4)
            s1.push_back('0');
    }
    setText(s1 + data);
    old = text();
}

void MyLineEdit::setValue(int bit_len, const LongInt &data) {
    setValue(bit_len, QString::fromStdString(data.to_string(16)));
}

void MyLineEdit::setValue(int bit_len, const Point &data) {
    if (data.get_inf())
        setValue(bit_len, "", true);
    else
        setValue(bit_len, QString::fromStdString(data.to_string(bit_len)), true);
}

void MyLineEdit::editedSlot() {
    if (text() == old)
        return;
    old = text();
    emit changedSignal();
}
