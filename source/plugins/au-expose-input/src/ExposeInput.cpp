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
#include "ExposeInput.h"

const QColor cItemColor(220, 120, 180);

ExposeInput::ExposeInput(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_pReferencedInputPort(nullptr)
{
    m_pOutput = addOutput("", Signal::Type_Float);
    createProperties();

    m_pNameItem = nullptr;
}

ExposeInput::~ExposeInput()
{
}

void ExposeInput::processStart()
{
}

void ExposeInput::processStop()
{
}

void ExposeInput::process()
{
    if (m_pReferencedInputPort) {
        m_pReferencedInputPort->update();
        m_pOutput->setFloatValue(m_pReferencedInputPort->value().asFloat);
    }
}

void ExposeInput::reset()
{
}

QGraphicsItem* ExposeInput::graphicsItem()
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

QColor ExposeInput::color() const
{
    return cItemColor;
}

int ExposeInput::flags() const
{
    return Flag_NoTitle | Flag_NoFrame;
}

QString ExposeInput::exposedInputName() const
{
    return m_pPropName->valueText();
}

void ExposeInput::setRefInputPort(InputPort *pInputPort)
{
    Q_ASSERT(pInputPort != nullptr);
    m_pReferencedInputPort = pInputPort;
}

void ExposeInput::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["name"] = m_pPropName->value();
}

void ExposeInput::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropName->setValue(data["name"]);
    AudioUnit::deserialize(data, pContext);
}

void ExposeInput::createProperties()
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
