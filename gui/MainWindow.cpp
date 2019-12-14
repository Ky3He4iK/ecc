//
// Created by ky3he4ik on 12/12/2019.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    layout = new QGridLayout();

    curveWidget = new CurveWidget();
    layout->addWidget(curveWidget, 0, 0);

    keysHolder = new KeysHolder();
    layout->addWidget(keysHolder, 0, 1);

    resultHolder = new ResultHolder();
    layout->addWidget(resultHolder, 1, 0, 1, 2);

    setLayout(layout);

    connect(curveWidget, &CurveWidget::encodingFinishedSignal, resultHolder, &ResultHolder::encodingFinishedSlot);
    connect(curveWidget, &CurveWidget::decodingFinishedSignal, resultHolder, &ResultHolder::decodingFinishedSlot);
    connect(resultHolder, &ResultHolder::encodeSignal, curveWidget, &CurveWidget::encodeSlot);
    connect(resultHolder, &ResultHolder::decodeSignal, curveWidget, &CurveWidget::decodeSlot);

    connect(curveWidget, &CurveWidget::keypairGeneratedSignal, keysHolder, &KeysHolder::keypairGenerated);
    connect(curveWidget, &CurveWidget::sharedGeneratedSignal, keysHolder, &KeysHolder::sharedGenerated);
    connect(curveWidget, &CurveWidget::publicByPrivateGeneratedSignal, keysHolder, &KeysHolder::publicByPrivateGenerated);
    connect(curveWidget, &CurveWidget::requestKeysSignal, keysHolder, &KeysHolder::emitKeys);
    connect(curveWidget, &CurveWidget::changeIntLenSignal, keysHolder, &KeysHolder::changeIntLen);

    connect(keysHolder, &KeysHolder::keys, curveWidget, &CurveWidget::receiveKeysSlot);
    connect(keysHolder, &KeysHolder::requestKeyGenerating, curveWidget, &CurveWidget::requestKeyGeneratingSlot);
    connect(keysHolder, &KeysHolder::requestPublicByPrivate, curveWidget, &CurveWidget::requestPublicByPrivateSlot);
    connect(keysHolder, &KeysHolder::requestSharedSecretGenerate, curveWidget, &CurveWidget::requestSharedSecretGenerate);
}
