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
#include <QSlider>
#include <QGraphicsProxyWidget>
#include <QGraphicsSimpleTextItem>
#include <QVBoxLayout>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "Slider.h"

const int cDefaultWidth = 164;
const int cDefaultHeight = 24;

Slider::Slider(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pSlider = nullptr;
    m_pOutput = addOutput();
    createProperties();
}

Slider::~Slider()
{
}

void Slider::processStart()
{
    m_pOutput->setValue(m_pPropValue->value().toFloat());
}

void Slider::processStop()
{
}

void Slider::process()
{
}

void Slider::reset()
{
}

QGraphicsItem* Slider::graphicsItem()
{
    if (m_pSlider == nullptr) {
        m_pSlider = new QSlider(Qt::Horizontal);
        m_pSlider->setTickPosition(QSlider::TicksBelow);
        QObject::connect(m_pSlider, &QSlider::valueChanged, [this](int pos) {
            onSliderValueChanged(pos);
        });

        updateSliderValues();
        updateSliderOrientation();
    }

    QGraphicsProxyWidget *pWidgetItem = new QGraphicsProxyWidget();
    pWidgetItem->setWidget(m_pSlider);

    return pWidgetItem;
}

int Slider::flags() const
{
    return Flag_NoFrame;
}

void Slider::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["value"] = m_pPropValue->value();
    data["min"] = m_pPropMin->value();
    data["max"] = m_pPropMax->value();
    data["orientation"] = m_pPropOrientation->value();
}

void Slider::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropMin->setValue(data["min"]);
    m_pPropMax->setValue(data["max"]);
    m_pPropValue->setValue(data["value"]);
    m_pPropOrientation->setValue(data["orientation"]);
    AudioUnit::deserialize(data, pContext);
}

void Slider::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

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

    QVariantList list;
    list << "Horizontal" << "Vertical";
    m_pPropOrientation = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Orientation");
    m_pPropOrientation->setAttribute("enumNames", list);
    m_pPropOrientation->setValue(0);

    QObject::connect(propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        QtVariantProperty *pVariantProperty = dynamic_cast<QtVariantProperty*>(pProperty);
        if (pVariantProperty == m_pPropOrientation) {
            updateSliderOrientation();
        } else {
            updateSliderValues();
        }
    });

    pRoot->addSubProperty(m_pPropValue);
    pRoot->addSubProperty(m_pPropMin);
    pRoot->addSubProperty(m_pPropMax);
    pRoot->addSubProperty(m_pPropSteps);
    pRoot->addSubProperty(m_pPropOrientation);

    updateSliderValues();
}

void Slider::updateSliderValues()
{
    if (m_pSlider == nullptr) {
        return;
    }

    double min = m_pPropMin->value().toDouble();
    double max = m_pPropMax->value().toDouble();
    double value = m_pPropValue->value().toDouble();
    int nSteps = m_pPropSteps->value().toInt();
    double step = (max - min) / nSteps;
    int pos = (value - min) / step;

    m_pSlider->setMinimum(0);
    m_pSlider->setMaximum(nSteps);
    m_pSlider->setValue(pos);

    m_pOutput->setValue(value);
}

void Slider::updateSliderOrientation()
{
    if (m_pSlider == nullptr) {
        return;
    }

    Qt::Orientation orientation = m_pPropOrientation->value().toInt() == 0 ? Qt::Horizontal : Qt::Vertical;

    // Swap width <-> height when changing the orientation
    QRect r = m_pSlider->geometry();
    r.setWidth(orientation == Qt::Horizontal ? cDefaultWidth : cDefaultHeight);
    r.setHeight(orientation == Qt::Horizontal ? cDefaultHeight : cDefaultWidth);

    m_pSlider->setOrientation(orientation);
    m_pSlider->setGeometry(r);
}

void Slider::onSliderValueChanged(int pos)
{
    double min = m_pPropMin->value().toDouble();
    double max = m_pPropMax->value().toDouble();
    int nSteps = m_pPropSteps->value().toInt();
    double step = (max - min) / nSteps;
    double value = min + step * pos;
    m_pPropValue->setValue(value);
}
