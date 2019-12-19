//
// Created by ky3he4ik on 12/12/2019.
//

#include "MainWindow.h"
#include "../curve/Curve_parameters.h"
#include "../curve/CurveManager.h"

#include <exception>

#include <QRegExpValidator>
#include <QFont>
#include <set>

#define LE_TO_LI(lineEdit) (LongInt((lineEdit)->text().toStdString(), 16))
#define LE_TO_P(lineEdit, curve) (Point((curve), (lineEdit)->text().toStdString()))
#define SAFE_BEGIN try {
#define SAFE_END } catch (std::exception &e) {}

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
    label_cypher = new QLabel("cyphered text:\n(in hex)");
    layout->addWidget(label_cypher, 11, 0);
    label_sign = new QLabel("sign:");
    layout->addWidget(label_sign, 12, 0);

    label_sign_verify = new QLabel();
    layout->addWidget(label_sign_verify, 12, 2);

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
    edit_cypher->setPlaceholderText("Insert encrypted message in hex form here");
    layout->addWidget(edit_cypher, 11, 1);
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
    int p = 0;
    for (auto &name: manager.getCurveNames()) {
        auto[curve, len] = manager.getCurve(name);
        lens.emplace(len);
        curve_options.push_back(new QRadioButton(QString::fromStdString(name)));
        if (name == "secp256k1") {
            selected_curve = p;
            (*curve_options.rbegin())->setChecked(true);
        }
        curves_layout->addWidget(*curve_options.rbegin());
        connect(*curve_options.rbegin(), &QPushButton::pressed, this, &MainWindow::groupCurvePressed);
        p++;
    }
    curve_options.push_back(new QRadioButton("custom"));
    curves_layout->addWidget(*curve_options.rbegin());
    connect(*curve_options.rbegin(), &QPushButton::pressed, this, &MainWindow::groupCurvePressed);

    group_len = new QGroupBox();
    buttons_layout->addWidget(group_len);
    auto len_layout = new QVBoxLayout();
    for (auto &len: lens) {
        len_options.push_back(new QRadioButton(QString::number(len)));
        len_layout->addWidget(*len_options.rbegin());
        connect(*len_options.rbegin(), &QPushButton::pressed, this, &MainWindow::groupLenPressed);
        if (len == selected_len)
            (*len_options.rbegin())->setChecked(true);
    }

    group_curve->setLayout(curves_layout);
    group_len->setLayout(len_layout);

    layout->addLayout(buttons_layout, 0, 2, layout->rowCount() - 1, 1);

    setLayout(layout);
    setMinimumWidth(1360);

    connect(edit_a, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_b, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_p, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_base_point, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);
    connect(edit_order, &MyLineEdit::changedSignal, this, &MainWindow::editCurveChanged);

    connect(edit_private, &MyLineEdit::changedSignal, this, &MainWindow::editPrivateChanged);
    connect(edit_other_public, &MyLineEdit::changedSignal, this, &MainWindow::editOtherChanged);

    connect(button_generate_key, &QPushButton::pressed, this, &MainWindow::buttonGenerateKeySlot);

    connect(button_encode, &QPushButton::pressed, this, &MainWindow::buttonEncodeSlot);
    connect(button_decode, &QPushButton::pressed, this, &MainWindow::buttonDecodeSlot);

    connect(button_load_curve, &QPushButton::pressed, this, &MainWindow::buttonLoadCurveSlot);
    connect(button_load_key_pair, &QPushButton::pressed, this, &MainWindow::buttonLoadKeyPairSlot);
    connect(button_load_other, &QPushButton::pressed, this, &MainWindow::buttonLoadOtherSlot);
    connect(button_save_curve, &QPushButton::pressed, this, &MainWindow::buttonSaveCurveSlot);
    connect(button_save_key_pair, &QPushButton::pressed, this, &MainWindow::buttonSaveKeyPairSlot);

    set_len(256);
    update_curve_edits();
    edit_private->setValue(256, ecc.get_private_key());
    edit_public->setValue(256, ecc.get_public_key());
    edit_other_public->setValue(256, Point());
    edit_shared->setValue(256, LongInt());
}

void MainWindow::set_len(int new_len) {
    SAFE_BEGIN

#define SET_LEN(edit, ind) { \
    (edit)->setInputMask(QString(">") + (is_for_point[(ind)] ? point_prefix : int_prefix) + input_mask); \
    if ((edit)->text().isEmpty()) \
        (edit)->setText(QString(2 + input_mask.size(), '0')); \
}

        selected_len = new_len;
        QString input_mask;
        for (int i = 0; i < new_len; i += 16)
            input_mask += "HHHH ";
        input_mask += ";O";
        auto point_prefix = "\\09 ";
        auto int_prefix = "   ";
        SET_LEN(edit_a, 0)
        SET_LEN(edit_b, 1)
        SET_LEN(edit_p, 2)
        SET_LEN(edit_base_point, 3)
        SET_LEN(edit_order, 4)
        SET_LEN(edit_private, 5)
        SET_LEN(edit_public, 6)
        SET_LEN(edit_other_public, 7)
        SET_LEN(edit_shared, 8)
    SAFE_END
}

void MainWindow::buttonGenerateKeySlot() {
    SAFE_BEGIN
        auto[priv, pub] = ECC::create_keys(ecc.get_parameters());
        std::cerr << priv.to_string(16) << ' ' << pub.to_string() << '\n';
        edit_private->setValue(selected_len, priv);
        edit_public->setValue(selected_len, pub);
        ecc.set_keys(priv, pub);
    SAFE_END
}

void MainWindow::buttonLoadCurveSlot() {
    SAFE_BEGIN
        if (file_lock_state != 0)
            return;
        file_lock(1);
        //todo
        file_unlock();
    SAFE_END
}

void MainWindow::buttonLoadKeyPairSlot() {
    SAFE_BEGIN
        if (file_lock_state != 0)
            return;
        file_lock(2);
        auto file_path = QFileDialog::getOpenFileName(this, "Select key", "", "Json keys (*.json.pub *.json.priv *.json.pair ;; All files (*)");
        if (!file_path.isNull()) {
            QFile file(file_path);
            auto json = nlohmann::json(file.readAll().toStdString());
            auto found = findInJson("private_key", json);
            if (found != 0)
                edit_private->setValue(selected_len, found);
        }
        file_unlock();
    SAFE_END
}

void MainWindow::buttonLoadOtherSlot() {
    SAFE_BEGIN
        if (file_lock_state != 0)
            return;
        file_lock(3);
//        file_selector->setFileMode(QFileDialog::FileMode::ExistingFile);
        //todo
        file_unlock();
    SAFE_END
}

void MainWindow::buttonSaveCurveSlot() {
    SAFE_BEGIN
        if (file_lock_state != 0)
            return;
        file_lock(4);
//        file_selector->setFileMode(QFileDialog::FileMode::AnyFile);
        //todo
        file_unlock();
    SAFE_END
}

void MainWindow::buttonSaveKeyPairSlot() {
    SAFE_BEGIN
        if (file_lock_state != 0)
            return;
        file_lock(5);
//        file_selector->setFileMode(QFileDialog::FileMode::AnyFile);
        //todo
        file_unlock();
    SAFE_END
}

void MainWindow::buttonEncodeSlot() {
    SAFE_BEGIN
        auto clear = edit_clear_text->toPlainText().toStdString();
        auto cypher = ecc.encode(clear);
        edit_cypher->setPlainText(QString::fromStdString(cypher));

        auto[sign1, sign2] = ecc.sign_msg(clear);
        edit_sign->setPlainText(QString::fromStdString(sign1.to_string(36) + ';' + sign2.to_string(36)));
    SAFE_END
}

void MainWindow::buttonDecodeSlot() {
    SAFE_BEGIN
        auto cypher = edit_cypher->toPlainText().toStdString();
        auto clear = ecc.decode(cypher);
        edit_clear_text->setPlainText(QString::fromStdString(clear));

        auto s = edit_sign->toPlainText().split(';');
        bool is_signed = s.size() == 2;
        if (is_signed) {
            Sign sign(LongInt(s[0].toStdString(), 36), LongInt(s[1].toStdString(), 36));
            is_signed = ECC::verify_msg(clear, sign, LE_TO_P(edit_other_public, ecc.get_curve()), ecc.get_parameters())
                        || ECC::verify_msg(clear, sign, ecc.get_public_key(), ecc.get_parameters());
        }

        label_sign_verify->setText(is_signed ? "Sign is correct!" : "Sign is incorrect");
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

void MainWindow::file_lock(int state) {
    SAFE_BEGIN
        file_lock_state = state;
        button_save_curve->setEnabled(false);
        button_load_key_pair->setEnabled(false);
        button_load_other->setEnabled(false);
        button_save_curve->setEnabled(false);
        button_save_key_pair->setEnabled(false);
    SAFE_END
}

void MainWindow::file_unlock() {
    SAFE_BEGIN
        file_lock_state = 0;
        button_save_curve->setEnabled(true);
        button_load_key_pair->setEnabled(true);
        button_load_other->setEnabled(true);
        button_save_curve->setEnabled(true);
        button_save_key_pair->setEnabled(true);
    SAFE_END
}

LongInt MainWindow::findInJson(const std::string &name, const nlohmann::json &node) {
    auto v = node.find(name);
    if (v != node.end())
        return LongInt((*v).get<std::string>(), 16);
    for (auto &item: node) {
        if (item.is_object()) {
            auto r = findInJson(name, item);
            if (r != 0)
                return r;
        }
    }
    return LongInt(0);
}
