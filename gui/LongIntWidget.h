//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_LONGINTWIDGET_H
#define ECC_LONGINTWIDGET_H

#include <QWidget>

class LongIntWidget : public QWidget {
Q_OBJECT

public:
    explicit LongIntWidget(const QString &label = "", QWidget *parent = Q_NULLPTR);

    [[nodiscard]] QString getContents() const;

    void setContents(const QString &new_content);

    void setEnabled(bool enabled);

signals:

    void contentChanged();
};


#endif //ECC_LONGINTWIDGET_H
