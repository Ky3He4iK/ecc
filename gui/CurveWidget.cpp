//
// Created by ky3he4ik on 12/12/2019.
//

#include "CurveWidget.h"

CurveWidget::CurveWidget(QWidget *) {

}

void CurveWidget::longIntChanged() {

}

void CurveWidget::loadCurveSlot() {

}

void CurveWidget::saveCurveSlot() {

}

void CurveWidget::curveSelectedSlot(bool) {

}

void CurveWidget::encodeSlot(const QString &cleartext) {

}

void CurveWidget::decodeSlot(const QString &cyphertext, const QString &sign) {

}


void CurveWidget::receiveKeysSlot(const QString &private_key, const QString &public_key,
                                  const QString &other_public_key, const QString &shared_secred) {

}

void CurveWidget::requestKeyGeneratingSlot() {

}

void CurveWidget::requestPublicByPrivateSlot(const QString &private_key) {

}

void CurveWidget::requestSharedSecretGenerate(const QString &private_key, const QString &public_key,
                                              const QString &other_public_key) {

}