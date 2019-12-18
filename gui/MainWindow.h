//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_MAINWINDOW_H
#define ECC_MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QString>
#include <QVector>
#include <QRadioButton>

#include "../curve/ECC.h"
#include "MyLineEdit.h"
#include <bitset>

class MainWindow : public QWidget {
Q_OBJECT

private:
    QGridLayout *layout;
    QVBoxLayout *buttons_layout;

    QLabel *label_a;
    QLabel *label_b;
    QLabel *label_p;
    QLabel *label_base_point;
    QLabel *label_order;
    QLabel *label_private;
    QLabel *label_public;
    QLabel *label_other_public;
    QLabel *label_shared;
    QLabel *label_clear_text;
    QLabel *label_cypher;
    QLabel *label_sign;
    QLabel *label_sign_verify;

    MyLineEdit *edit_a;
    MyLineEdit *edit_b;
    MyLineEdit *edit_p;
    MyLineEdit *edit_base_point;
    MyLineEdit *edit_order;
    MyLineEdit *edit_private;
    MyLineEdit *edit_public;
    MyLineEdit *edit_other_public;
    MyLineEdit *edit_shared;

    QTextEdit *edit_clear_text;
    QTextEdit *edit_cypher;
    QTextEdit *edit_sign;

    QPushButton *button_generate_key;
    QPushButton *button_load_curve;
    QPushButton *button_load_key_pair;
    QPushButton *button_load_other;
    QPushButton *button_save_curve;
    QPushButton *button_save_key_pair;
    QPushButton *button_encode;
    QPushButton *button_decode;

    QGroupBox *group_curve, *group_len;
    QVector<QRadioButton *> curve_options;
    QVector<QRadioButton *> len_options;

    ECC ecc;

    std::bitset<9> is_for_point = {0b011001000};
    int selected_curve = -1;
    int selected_len = 256;

    void set_len(int new_len);

    void update_curve_edits();

public:

    explicit MainWindow(QWidget * = Q_NULLPTR);

private slots:

    void buttonGenerateKeySlot();

    void buttonLoadCurveSlot();

    void buttonLoadKeyPairSlot();

    void buttonLoadOtherSlot();

    void buttonSaveCurveSlot();

    void buttonSaveKeyPairSlot();

    void buttonEncodeSlot();

    void buttonDecodeSlot();

    void groupCurvePressed();

    void groupLenPressed();

    void editCurveChanged();

    void editPrivateChanged();

    void editOtherChanged();

};


#endif //ECC_MAINWINDOW_H
