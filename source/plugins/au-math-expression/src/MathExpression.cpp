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
    m_pInput = addInput("x");
    m_pOutput = addOutput("f(x)");

    m_x = 0.0f;
    m_t = 0.0f;
    m_sampleRate = 44100.0f;    // does not matter, it will be reset on signal chain start.
    m_symbolTable.add_variable("x", m_x);
    m_symbolTable.add_variable("t", m_t);
    m_symbolTable.add_variable("sr", m_sampleRate);
    m_symbolTable.add_constants();

    m_expression.register_symbol_table(m_symbolTable);

    m_compiledOk = false;

    m_script = "x";
}

MathExpression::~MathExpression()
{
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
    data["expression"] = m_script;
    AudioUnit::serialize(data, pContext);
}

void MathExpression::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_script = data.value("expression").toString();
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
    m_x = m_pInput->value();
    m_t = m_timeStep * signalChain()->timeStep();
    float out = m_compiledOk ? m_expression.value() : 0.0f;
    m_pOutput->setValue(out);

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
    foreach (const QString &line, lines) {
        QString tr = line.trimmed();
        if (tr.left(1) != "#") {
            res = res.append(QString("%1\n").arg(line));
        }
    }
    return res;
}
