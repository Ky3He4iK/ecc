//
// Created by ky3he4ik on 18/12/2019.
//

#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *) {
    connect(this, &MyLineEdit::editingFinished, this, &MyLineEdit::changedSignal);
}

void MyLineEdit::setValue(int bit_len, const QString &data, char for_point) {
    QString res(bit_len / 16 * 4 + (for_point ? 2 : 0), '0');
    for (int i = data.size() - 1, j = res.size() - 1; i >= 0 && j >= 0; i--, j--)
        res[j] = data[i];
    if (for_point)
        res[1] = for_point;
    old = res;
    setText(res);
}

void MyLineEdit::setValue(int bit_len, const LongInt &data) {
    setValue(bit_len, QString::fromStdString(data.to_string(16)));
}

void MyLineEdit::setValue(int bit_len, const Point &data) {
    setValue(bit_len, QString::fromStdString(data.get_x().to_string(16)), '2' + data.get_y().is_odd());
}

void MyLineEdit::editedSlot() {
    if (text() == old)
        return;
    old = text();
    emit changedSignal();
}
