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

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QGraphicsWidget>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "ExposedInput.h"

const QColor cItemColor(220, 120, 180);

ExposedInput::ExposedInput(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_pReferencedInputPort(nullptr)
{
    m_pOutput = addOutput();
    createProperties();

    m_pNameItem = nullptr;
}

ExposedInput::~ExposedInput()
{
}

void ExposedInput::processStart()
{
}

void ExposedInput::processStop()
{
}

void ExposedInput::process()
{
    if (m_pReferencedInputPort) {
        m_pReferencedInputPort->update();
        m_pOutput->setValue(m_pReferencedInputPort->value());
    }
}

void ExposedInput::reset()
{
}

QGraphicsItem* ExposedInput::graphicsItem()
{
    if (m_pNameItem == nullptr) {
        m_pNameItem = new QGraphicsSimpleTextItem();
        QBrush brush(QColor(160, 60, 60));
        m_pNameItem->setBrush(brush);

        QFont font = m_pNameItem->font();
        font.setBold(true);
        m_pNameItem->setFont(font);

        m_pNameItem->setText(m_pPropName->valueText() + " ");
    }
    return m_pNameItem;
}

QColor ExposedInput::color() const
{
    return cItemColor;
}

int ExposedInput::flags() const
{
    return Flag_NoTitle | Flag_NoFrame;
}

QString ExposedInput::exposedInputName() const
{
    return m_pPropName->valueText();
}

void ExposedInput::setRefInputPort(InputPort *pInputPort)
{
    Q_ASSERT(pInputPort != nullptr);
    m_pReferencedInputPort = pInputPort;
}

void ExposedInput::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["name"] = m_pPropName->value();
}

void ExposedInput::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropName->setValue(data["name"]);
    AudioUnit::deserialize(data, pContext);
}

void ExposedInput::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
    m_pPropName = propertyManager()->addProperty(QVariant::String, "Name");
    m_pPropName->setValue("input");

    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        // Update text item with the value for the property.
        QtVariantProperty *pV = dynamic_cast<QtVariantProperty*>(pProperty);
        if (pV == m_pPropName) {
            if (m_pNameItem != nullptr) {
                m_pNameItem->setText(pV->valueText() + " ");
            }
        }
    });
    pRoot->addSubProperty(m_pPropName);
}
