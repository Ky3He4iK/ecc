//
// Created by ky3he4ik on 12/12/2019.
//

#include "MainWindow.h"
#include "InfoDialog.h"
#include "../curve/Curve_parameters.h"
#include "../curve/CurveManager.h"

#include <QRegExpValidator>
#include <QFont>
#include <set>

#define LE_TO_LI(lineEdit) (LongInt((lineEdit)->text().toStdString(), 16))
#define LE_TO_P(lineEdit, curve) (Point((curve), (lineEdit)->text().toStdString()))
#define SAFE_BEGIN try {
#define SAFE_END } catch (int) {}

MainWindow::MainWindow(QWidget *) : ecc(ECC(Curve_parameters::curve_secp256k1())) {
    auto font = QFont("DejaVu");
    font.setStyleHint(QFont::StyleHint::Monospace);
    setFont(font);
    layout = new QGridLayout();
    buttons_layout = new QVBoxLayout();

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
    label_other_public = new QLabel("`Bob`'s public key:");
    layout->addWidget(label_other_public, 7, 0);
    label_shared = new QLabel("shared private key:");
    layout->addWidget(label_shared, 8, 0);
    label_clear_text = new QLabel("clear text:");
    layout->addWidget(label_clear_text, 9, 0);
    label_cypher = new QLabel("cyphered text:");
    layout->addWidget(label_cypher, 10, 0);
    label_base64 = new QLabel("cyphered text:\n(base 64)");
    layout->addWidget(label_base64, 11, 0);
    label_sign = new QLabel("sign:");
    layout->addWidget(label_sign, 12, 0);

    edit_a = new MyLineEdit();
    if (is_for_point[0])
        edit_a->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_a, 0, 1);
    edit_b = new MyLineEdit();
    if (is_for_point[1])
        edit_b->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_b, 1, 1);
    edit_p = new MyLineEdit();
    if (is_for_point[2])
        edit_p->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_p, 2, 1);
    edit_base_point = new MyLineEdit();
    if (is_for_point[3])
        edit_base_point->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_base_point, 3, 1);
    edit_order = new MyLineEdit();
    if (is_for_point[4])
        edit_order->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_order, 4, 1);
    edit_private = new MyLineEdit();
    if (is_for_point[5])
        edit_private->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_private, 5, 1);
    edit_public = new MyLineEdit();
    if (is_for_point[6])
        edit_public->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    edit_public->setReadOnly(true);
    layout->addWidget(edit_public, 6, 1);
    edit_other_public = new MyLineEdit();
    if (is_for_point[7])
        edit_other_public->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
    layout->addWidget(edit_other_public, 7, 1);
    edit_shared = new MyLineEdit();
    if (is_for_point[8])
        edit_shared->setValidator(new QRegExpValidator(QRegExp("0[2-3].*")));
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

    button_generate_key = new QPushButton("Generate new keypair");
    buttons_layout->addWidget(button_generate_key);
    button_load_curve = new QPushButton("Load curve from file");
    buttons_layout->addWidget(button_load_curve);
    button_load_key_pair = new QPushButton("Load private key from file");
    buttons_layout->addWidget(button_load_key_pair);
    button_load_other = new QPushButton("Load `Bob`'s key from file");
    buttons_layout->addWidget(button_load_other);
    button_save_curve = new QPushButton("Save curve to file");
    buttons_layout->addWidget(button_save_curve);
    button_save_key_pair = new QPushButton("Save key pair to file");
    buttons_layout->addWidget(button_save_key_pair);
    button_encode = new QPushButton("Encode");
    buttons_layout->addWidget(button_encode);
    button_decode = new QPushButton("Decode");
    buttons_layout->addWidget(button_decode);

    group_curve = new QGroupBox();
    buttons_layout->addWidget(group_curve);
    auto curves_layout = new QVBoxLayout();
    auto manager = CurveManager::getInstance();
    std::set<int> lens;
    for (auto &name: manager.getCurveNames()) {
        auto[curve, len] = manager.getCurve(name);
        if (selected_curve == -1) {
            selected_curve = 0;
            set_len(len);
            ecc = ECC(curve);
            update_curve_edits();
            buttonGenerateKeySlot();
        }
        lens.emplace(len);
        curve_options.push_back(new QPushButton(QString::fromStdString(name)));
        curves_layout->addWidget(*curve_options.rbegin());
        connect(*curve_options.rbegin(), &QPushButton::pressed, this, &MainWindow::groupCurvePressed);
    }
    curve_options.push_back(new QPushButton("custom"));
    curves_layout->addWidget(*curve_options.rbegin());
    connect(*curve_options.rbegin(), &QPushButton::pressed, this, &MainWindow::groupCurvePressed);

    group_len = new QGroupBox();
    buttons_layout->addWidget(group_len);
    auto len_layout = new QVBoxLayout();
    for (auto &len: lens) {
        len_options.push_back(new QPushButton(QString::number(len)));
        len_layout->addWidget(*len_options.rbegin());
        connect(*len_options.rbegin(), &QPushButton::pressed, this, &MainWindow::groupLenPressed);
    }

    group_curve->setLayout(curves_layout);
    group_len->setLayout(len_layout);

    layout->addLayout(buttons_layout, 0, 2, layout->rowCount(), 1);

    setLayout(layout);
    setMinimumWidth(1360);

    connect(edit_a, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_b, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_p, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_base_point, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_order, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);

    connect(edit_private, &MyLineEdit::changedSignal, this, &MainWindow::editPrivateChanged);
    connect(edit_other_public, &MyLineEdit::changedSignal, this, &MainWindow::editOtherChanged);

    connect(edit_cypher, &QTextEdit::textChanged, this, &MainWindow::editCypherChangedSlot);
    connect(edit_base64, &QTextEdit::textChanged, this, &MainWindow::editBase64ChangedSlot);

    connect(button_generate_key, &QPushButton::pressed, this, &MainWindow::buttonGenerateKeySlot);

    connect(button_encode, &QPushButton::pressed, this, &MainWindow::buttonEncodeSlot);
    connect(button_decode, &QPushButton::pressed, this, &MainWindow::buttonDecodeSlot);

//    void buttonLoadCurveSlot();
//
//    void buttonLoadKeyPairSlot();
//
//    void buttonLoadOtherSlot();
//
//    void buttonSaveCurveSlot();
//
//    void buttonSaveKeyPairSlot();
//
//    void buttonEncodeSlot();
//
//    void buttonDecodeSlot();
//
//    void groupCurvePressed();
//
//    void groupLenPressed();
}

void MainWindow::set_len(int new_len) {
    SAFE_BEGIN
        selected_len = new_len;
        QString input_mask;
        for (int i = 0; i < new_len; i += 16)
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
    SAFE_END
}


void MainWindow::buttonGenerateKeySlot() {
    SAFE_BEGIN
        auto[priv, pub] = ECC::create_keys(ecc.get_parameters());
        edit_private->setValue(selected_len, priv);
        editPrivateChanged();
    SAFE_END
}

void MainWindow::buttonLoadCurveSlot() {
    SAFE_BEGIN
        //todo
    SAFE_END
}

void MainWindow::buttonLoadKeyPairSlot() {
    SAFE_BEGIN
        //todo
    SAFE_END
}

void MainWindow::buttonLoadOtherSlot() {
    SAFE_BEGIN
        //todo
    SAFE_END
}

void MainWindow::buttonSaveCurveSlot() {
    SAFE_BEGIN
        //todo
    SAFE_END
}

void MainWindow::buttonSaveKeyPairSlot() {
    SAFE_BEGIN
        //todo
    SAFE_END
}

void MainWindow::buttonEncodeSlot() {
    SAFE_BEGIN
        auto clear = edit_clear_text->toPlainText().toStdString();
        edit_cypher->setPlainText(QString::fromStdString(ecc.encode(clear)));
        editCypherChangedSlot();

        auto[sign1, sign2] = ecc.sign_msg(clear);
        edit_sign->setPlainText(QString::fromStdString(sign1.to_string(36) + ';' + sign2.to_string(36)));
    SAFE_END
}

void MainWindow::buttonDecodeSlot() {
    SAFE_BEGIN
        auto cypher = edit_clear_text->toPlainText().toStdString();
        auto clear = ecc.encode(cypher);
        edit_clear_text->setPlainText(QString::fromStdString(clear));

        auto s = edit_sign->toPlainText().split(';');
        bool is_signed = s.size() == 2;
        if (is_signed) {
            Sign sign(LongInt(s[0].toStdString(), 36), LongInt(s[1].toStdString(), 36));
            is_signed = ECC::verify_msg(clear, sign, LE_TO_P(edit_other_public, ecc.get_curve()), ecc.get_parameters())
                        || ECC::verify_msg(clear, sign, ecc.get_public_key(), ecc.get_parameters());
        }

        InfoDialog res(is_signed ? "Sign is correct!" : "Sign is incorrect", this);
        res.open();
    SAFE_END
}

void MainWindow::editCypherChangedSlot() {
    SAFE_BEGIN
        if (lock)
            return;
        lock = true;
        edit_base64->setPlainText(edit_cypher->toPlainText().toUtf8().toBase64());
        lock = false;
    SAFE_END
}

void MainWindow::editBase64ChangedSlot() {
    SAFE_BEGIN
        if (lock)
            return;
        lock = true;
        edit_cypher->setPlainText(QString(QByteArray::fromBase64(edit_base64->toPlainText().toUtf8())));
        lock = false;
    SAFE_END
}

void MainWindow::groupCurvePressed() {
    SAFE_BEGIN
        for (int i = 0; i < curve_options.size(); i++)
            if (curve_options[i]->isChecked()) {
                if (i != selected_curve) {
                    selected_curve = i;
                    if (i != curve_options.size() - 1) {
                        auto[param, len] = CurveManager::getInstance().getCurve(curve_options[i]->text().toStdString());
                        ecc = ECC(param);
                        set_len(len);
                        update_curve_edits();
                    }
                }
                return;
            }
    SAFE_END
}

void MainWindow::groupLenPressed() {
    SAFE_BEGIN
        for (auto &len_option : len_options)
            if (len_option->isChecked()) {
                if (len_option->text().toInt() != selected_len) {
                    set_len(len_option->text().toInt());
                    (*curve_options.rbegin())->setChecked(true);
                    groupCurvePressed();
                }
                return;
            }
    SAFE_END
}

void MainWindow::editCurveChanged() {
    SAFE_BEGIN
        ecc = ECC(Curve_parameters(edit_a->text().toStdString(), edit_b->text().toStdString(),
                                   edit_p->text().toStdString(),
                                   edit_base_point->text().toStdString(), edit_order->text().toStdString()));
        editOtherChanged();
    SAFE_END
}

void MainWindow::editPrivateChanged() {
    SAFE_BEGIN
        ecc.set_private_key(LE_TO_LI(edit_private));
        edit_public->setValue(selected_len, ecc.get_public_key());
    SAFE_END
}

void MainWindow::editOtherChanged() {
    SAFE_BEGIN
        edit_shared->setValue(selected_len, ecc.set_shared_secret(LE_TO_P(edit_other_public, ecc.get_curve())));
    SAFE_END
}

void MainWindow::update_curve_edits() {
    edit_a->setValue(selected_len, ecc.get_parameters().get_a());
    edit_b->setValue(selected_len, ecc.get_parameters().get_b());
    edit_p->setValue(selected_len, ecc.get_parameters().get_p());
    edit_base_point->setValue(selected_len, ecc.get_parameters().get_base_point());
    edit_order->setValue(selected_len, ecc.get_parameters().get_order());
}
