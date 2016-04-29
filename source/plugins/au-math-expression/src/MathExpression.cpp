/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include "Application.h"
#include "ISignalChain.h"
#include "ExprEditorDialog.h"
#include "MathExpression.h"


MathExpression::MathExpression(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_t = 0.0f;
    m_sampleRate = 44100.0f;    // does not matter, it will be reset on signal chain start.
    m_symbolTable.add_variable("t", m_t);
    m_symbolTable.add_variable("sr", m_sampleRate);
    m_symbolTable.add_constants();

    m_expression.register_symbol_table(m_symbolTable);

    m_compiledOk = false;
    m_script = "y := 0;";
}

MathExpression::~MathExpression()
{
}

void MathExpression::createPorts(int nInputs, int nOutputs)
{
    for (int i = 0; i < nInputs; ++i) {
        QString name = nInputs > 1 ? QString("x[%1]").arg(i) : "x";
        InputPort *pInput = addInput(name);
        m_inputs.append(pInput);
        m_xVector.push_back(0.0f);
    }

    for (int i = 0; i < nOutputs; ++i) {
        QString name = nOutputs > 1 ? QString("y[%1]").arg(i) : "y";
        OutputPort *pOutput = addOutput(name);
        m_outputs.append(pOutput);
        m_yVector.push_back(0.0f);
    }

    if (nInputs > 1) {
        m_symbolTable.add_vector("x", m_xVector);
    } else if (nInputs == 1) {
        m_symbolTable.add_variable("x", m_xVector[0]);
    }

    if (nOutputs > 1) {
        m_symbolTable.add_vector("y", m_yVector);
    } else {
        m_symbolTable.add_variable("y", m_yVector[0]);
    }

}

QGraphicsItem* MathExpression::graphicsItem()
{
    QPushButton *pEdit = new QPushButton("Edit...");
    pEdit->setFlat(true);
    QObject::connect(pEdit, &QPushButton::clicked, [this]() {
        showScriptEditor();
    });
    QGraphicsProxyWidget *pWidgetItem = new QGraphicsProxyWidget();
    pWidgetItem->setWidget(pEdit);
    return pWidgetItem;
}

void MathExpression::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["nInputs"] = m_inputs.count();
    data["nOutputs"] = m_outputs.count();
    data["expression"] = m_script;
    AudioUnit::serialize(data, pContext);
}

void MathExpression::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    int nInputs = data.value("nInputs", 1).toInt();
    int nOutputs = data.value("nOutputs", 1).toInt();
    m_script = data.value("expression").toString();

    createPorts(nInputs, nOutputs);

    AudioUnit::deserialize(data, pContext);
}

void MathExpression::processStart()
{
    // Initialize and compile expression
    Parser parser;
    QString script = removeScriptComments(m_script);
    m_compiledOk = parser.compile(script.toStdString(), m_expression);
    if (!m_compiledOk) {
        qCritical() << "Unable to evaluate expression";
        qDebug() << script;
    }

    m_sampleRate = signalChain()->sampleRate();

    m_timeStep = 0;
}

void MathExpression::processStop()
{
}

void MathExpression::process()
{
    if (!m_compiledOk) {
        return;
    }

    // Copy input values
    for (int i = 0; i < m_inputs.count(); i++) {
        m_xVector[i] = m_inputs.at(i)->value();
    }

    // Set current time
    m_t = m_timeStep * signalChain()->timeStep();

    // Evaluate the expression
    m_expression.value();

    // Copy outputs from the script
    for (int i = 0; i < m_outputs.count(); i++) {
        m_outputs[i]->setValue(m_yVector.at(i));
    }

    // Advance time
    ++m_timeStep;
}

void MathExpression::reset()
{
}

void MathExpression::showScriptEditor()
{
    if (isStarted()) {
        return;
    }

    ExprEditorDialog dlg;
    dlg.setScript(m_script);

    if (dlg.exec() == QDialog::Accepted) {
        m_script = dlg.script();
    }
}

QString MathExpression::removeScriptComments(const QString &src)
{
    QStringList lines = src.split("\n", QString::SkipEmptyParts);
    QString res;
    for (const QString &line : lines) {
        QString tr = line.trimmed();
        if (tr.left(1) != "#") {
            res = res.append(QString("%1\n").arg(line));
        }
    }
    return res;
}
