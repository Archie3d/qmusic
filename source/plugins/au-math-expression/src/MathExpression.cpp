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
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "MathExpression.h"


MathExpression::MathExpression(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInput = addInput("x");
    m_pOutput = addOutput("f(x)");

    m_x = 0.0f;
    m_t = 0.0f;
    m_symbolTable.add_variable("x", m_x);
    m_symbolTable.add_variable("t", m_t);
    m_symbolTable.add_constants();

    m_expression.register_symbol_table(m_symbolTable);

    m_compiledOk = false;

    createProperties();
}

MathExpression::~MathExpression()
{
}

void MathExpression::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["expression"] = m_pPropExpression->value();
    AudioUnit::serialize(data, pContext);
}

void MathExpression::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropExpression->setValue(data["expression"]);
    AudioUnit::deserialize(data, pContext);
}

void MathExpression::processStart()
{
    // Initialize and compile expression
    QString code = m_pPropExpression->valueText();
    Parser parser;
    m_compiledOk = parser.compile(code.toStdString(), m_expression);
    if (!m_compiledOk) {
        qCritical() << "Unable to evaluate expression";
        qDebug() << code;
    }

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

void MathExpression::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropExpression = propertyManager()->addProperty(QVariant::String, "Expression");
    m_pPropExpression->setValue("x");
    pRoot->addSubProperty(m_pPropExpression);

}
