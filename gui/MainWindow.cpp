//
// Created by ky3he4ik on 12/12/2019.
//

#include <QRegExpValidator>
#include "MainWindow.h"
#include "../curve/Curve_parameters.h"

MainWindow::MainWindow(QWidget *) : ecc(ECC(Curve_parameters::curve_secp256k1())) {
    layout = new QGridLayout();

    label_a = new QLabel("a:");
    layout->addWidget(label_a, 0, 0);
    label_b = new QLabel("b:");
    layout->addWidget(label_b, 1, 0);
    label_p = new QLabel("p:");
    layout->addWidget(label_p, 2, 0);
    label_base_point = new QLabel("base point:");
    layout->addWidget(label_base_point, 3, 0);
    label_order = new QLabel("curve order:");
    layout->addWidget(label_order, 4, 0);
    label_private = new QLabel("private key:");
    layout->addWidget(label_private, 5, 0);
    label_public = new QLabel("public key:");
    layout->addWidget(label_public, 6, 0);
    label_other_public = new QLabel("other's public key:");
    layout->addWidget(label_other_public, 7, 0);
    label_shared = new QLabel("shared private key:");
    layout->addWidget(label_shared, 8, 0);
    label_clear_text = new QLabel("clear text:");
    layout->addWidget(label_clear_text, 9, 0);
    label_cypher = new QLabel("cyphered text:");
    layout->addWidget(label_cypher, 10, 0);
    label_base64 = new QLabel("cyphered text:\n(base 64)");
    layout->addWidget(label_base64, 11, 0);
    label_sign = new QLabel();
    layout->addWidget(label_sign, 12, 0);

    edit_a = new QLineEdit();
    if (is_for_point[0])
        edit_a->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_a, 0, 1);
    edit_b = new QLineEdit();
    if (is_for_point[1])
        edit_b->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_b, 1, 1);
    edit_p = new QLineEdit();
    if (is_for_point[2])
        edit_p->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_p, 2, 1);
    edit_base_point = new QLineEdit();
    if (is_for_point[3])
        edit_base_point->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_base_point, 3, 1);
    edit_order = new QLineEdit();
    if (is_for_point[4])
        edit_order->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_order, 4, 1);
    edit_private = new QLineEdit();
    if (is_for_point[5])
        edit_private->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_private, 5, 1);
    edit_public = new QLineEdit();
    if (is_for_point[6])
        edit_public->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    edit_public->setReadOnly(true);
    layout->addWidget(edit_public, 6, 1);
    edit_other_public = new QLineEdit();
    if (is_for_point[7])
        edit_other_public->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    layout->addWidget(edit_other_public, 7, 1);
    edit_shared = new QLineEdit();
    if (is_for_point[8])
        edit_shared->setValidator(new QRegExpValidator(QRegExp("0[2-4].*")));
    edit_shared->setReadOnly(true);
    layout->addWidget(edit_shared, 8, 1);

    edit_clear_text = new QTextEdit();
    edit_clear_text->setPlaceholderText("Insert message you want to encode here");
    layout->addWidget(edit_clear_text, 9, 1);
    edit_cypher = new QTextEdit();
    edit_cypher->setPlaceholderText("Insert message you want to decode here");
    layout->addWidget(edit_cypher, 10, 1);
    edit_base64 = new QTextEdit();
    edit_base64->setPlaceholderText("Or insert encrypted message in base64 here");
    layout->addWidget(edit_base64, 11, 1);
    edit_sign = new QTextEdit();
    edit_sign->setPlaceholderText("Field for signing message");
    layout->addWidget(edit_sign, 12, 1);

    button_load_curve = new QPushButton("Load curve from file");
    layout->addWidget(button_load_curve, 6, 2);
    button_load_key_pair = new QPushButton("Load private key from file");
    layout->addWidget(button_load_key_pair, 7, 2);
    button_load_other = new QPushButton("Load other key from file");
    layout->addWidget(button_load_other, 8, 2);
    button_save_curve = new QPushButton("Save curve to file");
    layout->addWidget(button_save_curve, 9, 2);
    button_save_key_pair = new QPushButton("Save key pair to file");
    layout->addWidget(button_save_key_pair, 10, 2);
    button_encode = new QPushButton("Encode");
    layout->addWidget(button_encode, 11, 2);
    button_decode = new QPushButton("Decode");
    layout->addWidget(button_decode, 12, 2);

    group_curve = new QGroupBox();
    layout->addWidget(group_curve, 0, 2, 3, 1);

    group_len = new QGroupBox();
    layout->addWidget(group_len, 3, 2, 3, 1);

    setLayout(layout);
    setMinimumWidth(1300);
    set_len(256);
}

void MainWindow::set_len(int new_len) {
    QString input_mask;
    for (int i = 0; i < new_len + 15; i += 16)
        input_mask += "HHHH ";
    input_mask += ";0";
    auto point_prefix = "\\09 ";
    auto int_prefix = "   ";
    edit_a->setInputMask(QString(">") + (is_for_point[0] ? point_prefix : int_prefix) + input_mask);
    edit_b->setInputMask(QString(">") + (is_for_point[1] ? point_prefix : int_prefix) + input_mask);
    edit_p->setInputMask(QString(">") + (is_for_point[2] ? point_prefix : int_prefix) + input_mask);
    edit_base_point->setInputMask(QString(">") + (is_for_point[3] ? point_prefix : int_prefix) + input_mask);
    edit_order->setInputMask(QString(">") + (is_for_point[4] ? point_prefix : int_prefix) + input_mask);
    edit_private->setInputMask(QString(">") + (is_for_point[5] ? point_prefix : int_prefix) + input_mask);
    edit_public->setInputMask(QString(">") + (is_for_point[6] ? point_prefix : int_prefix) + input_mask);
    edit_other_public->setInputMask(QString(">") + (is_for_point[7] ? point_prefix : int_prefix) + input_mask);
    edit_shared->setInputMask(QString(">") + (is_for_point[8] ? point_prefix : int_prefix) + input_mask);
}


void MainWindow::buttonLoadCurveSlot() {

}

void MainWindow::buttonLoadKeyPairSlot() {

}

void MainWindow::buttonLoadOtherSlot() {

}

void MainWindow::buttonSaveCurveSlot() {

}

void MainWindow::buttonSaveKeyPairSlot() {

}

void MainWindow::buttonEncodeSlot() {

}

void MainWindow::buttonDecodeSlot() {

}

void MainWindow::editCypherChangedSlot() {

}

void MainWindow::editBase64ChangedSlot() {

}

void MainWindow::groupCurvePressed() {

}

void MainWindow::groupLenPressed() {

}

void MainWindow::editCurveChanged() {

}

void MainWindow::editPrivateChanged() {

}

void MainWindow::editOtherChanged() {

}
