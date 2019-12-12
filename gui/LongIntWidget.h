//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_LONGINTWIDGET_H
#define ECC_LONGINTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class LongIntWidget : public QWidget {
Q_OBJECT

private:
    QLineEdit *line;
    QLabel *label;
    QHBoxLayout *layout;

public:
    explicit LongIntWidget(const QString &_label = "", int input_len = 256, bool is_for_key = false,
                           QWidget *parent = Q_NULLPTR);

    [[nodiscard]] QString getContents() const;

    void setContents(const QString &new_content);

    void setEnabled(bool enabled);

private slots:
    void contentChangedSlot(const QString &);

signals:

    void contentChangedSignal();
};


#endif //ECC_LONGINTWIDGET_H
