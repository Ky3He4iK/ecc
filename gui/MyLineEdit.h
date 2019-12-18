//
// Created by ky3he4ik on 18/12/2019.
//

#ifndef ECC_MYLINEEDIT_H
#define ECC_MYLINEEDIT_H

#include <QLineEdit>
#include <QString>

#include "../curve/Point.h"

class MyLineEdit : public QLineEdit {
Q_OBJECT
private:
    QString old = "";

public:
    explicit MyLineEdit(QWidget * = Q_NULLPTR);

    void setValue(int bit_len, const QString &data, bool is_point = false);

    void setValue(int bit_len, const LongInt &data);

    void setValue(int bit_len, const Point &data);

private slots:

    void editedSlot();

signals:

    void changedSignal();

};


#endif //ECC_MYLINEEDIT_H
