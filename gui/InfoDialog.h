//
// Created by ky3he4ik on 18/12/2019.
//

#ifndef ECC_INFODIALOG_H
#define ECC_INFODIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class InfoDialog: public QDialog {
    Q_OBJECT

private:
    QPushButton *ok;
    QLabel *text;
public:
    InfoDialog(const QString &_text, QWidget *);

private slots:
    void ok_pressed();

};


#endif //ECC_INFODIALOG_H
