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
#include <QDial>
#include <QGraphicsProxyWidget>
#include "Application.h"
#include "ISignalChain.h"
#include "AudioUnitPlugin.h"
#include "Dial.h"

const int cDefaultSize = 48;

Dial::Dial(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pDial = nullptr;
    m_pOutput = addOutput();
    createProperties();
}

Dial::~Dial()
{
}

void Dial::processStart()
{
    m_pOutput->setValue(m_pPropValue->value().toFloat());
}

void Dial::processStop()
{
}

void Dial::process()
{
}

void Dial::reset()
{
}

QGraphicsItem* Dial::graphicsItem()
{
    if (m_pDial == nullptr) {
        m_pDial = new QDial();
        QObject::connect(m_pDial, &QDial::valueChanged, [this](int pos) {
            onDialValueChanged(pos);
        });

        QRect r = m_pDial->geometry();
        r.setWidth(cDefaultSize);
        r.setHeight(cDefaultSize);
        m_pDial->setGeometry(r);
        m_pDial->setNotchesVisible(true);
        m_pDial->setWrapping(false);
        updateDialValues();
    }

    QGraphicsProxyWidget *pWidgetItem = new QGraphicsProxyWidget();
    pWidgetItem->setWidget(m_pDial);

    return pWidgetItem;
}

int Dial::flags() const
{
    return Flag_NoFrame | Flag_NoIcon;
}

QString Dial::title() const
{
    return m_pPropLabel->valueText();
}

void Dial::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["label"] = m_pPropLabel->value();
    data["value"] = m_pPropValue->value();
    data["min"] = m_pPropMin->value();
    data["max"] = m_pPropMax->value();
}

void Dial::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropMin->setValue(data["min"]);
    m_pPropMax->setValue(data["max"]);
    m_pPropValue->setValue(data["value"]);
    m_pPropLabel->setValue(data["label"]);
    AudioUnit::deserialize(data, pContext);
}

void Dial::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropLabel = propertyManager()->addProperty(QVariant::String, "Label");
    m_pPropLabel->setValue(plugin()->name());

    m_pPropValue = propertyManager()->addProperty(QVariant::Double, "Value");
    m_pPropValue->setValue(0.0);
    m_pPropValue->setAttribute("decimals", 2);
    m_pPropValue->setAttribute("singleStep", 0.1);

    m_pPropMin = propertyManager()->addProperty(QVariant::Double, "Mininum");
    m_pPropMin->setValue(0.0);
    m_pPropMin->setAttribute("decimals", 2);
    m_pPropMin->setAttribute("signleStep", 0.1);


    m_pPropMax = propertyManager()->addProperty(QVariant::Double, "Maximum");
    m_pPropMax->setValue(1.0);
    m_pPropMax->setAttribute("decimals", 2);
    m_pPropMax->setAttribute("signleStep", 0.1);

    m_pPropSteps = propertyManager()->addProperty(QVariant::Int, "Steps");
    m_pPropSteps->setValue(10);
    m_pPropSteps->setAttribute("minimum", 2);

    QObject::connect(propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        updateDialValues();
    });

    pRoot->addSubProperty(m_pPropLabel);
    pRoot->addSubProperty(m_pPropValue);
    pRoot->addSubProperty(m_pPropMin);
    pRoot->addSubProperty(m_pPropMax);
    pRoot->addSubProperty(m_pPropSteps);

    updateDialValues();
}

void Dial::updateDialValues()
{
    if (m_pDial == nullptr) {
        return;
    }

    double min = m_pPropMin->value().toDouble();
    double max = m_pPropMax->value().toDouble();
    double value = m_pPropValue->value().toDouble();
    int nSteps = m_pPropSteps->value().toInt();
    double step = (max - min) / nSteps;
    int pos = (value - min) / step;

    m_pDial->setMinimum(0);
    m_pDial->setMaximum(nSteps);
    m_pDial->setValue(pos);

    m_pOutput->setValue(value);
}

void Dial::onDialValueChanged(int pos)
{
    double min = m_pPropMin->value().toDouble();
    double max = m_pPropMax->value().toDouble();
    int nSteps = m_pPropSteps->value().toInt();
    double step = (max - min) / nSteps;
    double value = min + step * pos;
    m_pPropValue->setValue(value);
}
