//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_LONGINTWIDGET_H
#define ECC_LONGINTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollArea>

class LongIntWidget : public QWidget {
Q_OBJECT

private:
    QLineEdit *line;
    QLabel *label;
    QHBoxLayout *layout;
    bool is_for_point;

public:
    explicit LongIntWidget(const QString &_label = "", bool _is_for_point = false,
                           QWidget *parent = Q_NULLPTR);

    [[nodiscard]] QString getContents() const;

    void setContents(const QString &new_content);

//    void setEnabled(bool enabled);

    void setInputLen(int input_len = 256);

private slots:

    void contentChangedSlot();

signals:

    void contentChangedSignal();
};


#endif //ECC_LONGINTWIDGET_H
