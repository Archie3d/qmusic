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
#include "AudioDevicesManager.h"
#include "MidiNote.h"
#include "MidiDevice.h"
#include "MidiInputDevice.h"
#include "ISignalChain.h"
#include "MidiInCtrl.h"

QMap<QString, MidiMessage::Controller> cNameToController {
    {"Volume", MidiMessage::Controller_MainVolume},
    {"Modulation", MidiMessage::Controller_Modulation},
    {"Breath", MidiMessage::Controller_Breath},
    {"Foot", MidiMessage::Controller_Foot},
    {"Balance", MidiMessage::Controller_Balance},
    {"Pan", MidiMessage::Controller_Pan},
    {"Sustain pedal", MidiMessage::Controller_SustainPedal},
    {"Portamento", MidiMessage::Controller_Portamento},
    {"Sostenuto", MidiMessage::Controller_Sostenuto},
    {"Soft pedal", MidiMessage::Controller_SoftPedal}
};

const QColor cDefaultColor(230, 240, 210);

MidiInCtrl::MidiInCtrl(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutputValue = addOutput();

    m_pValueItem = nullptr;
    m_controllerValue = 100;

    Application::instance()->audioDevicesManager()->midiInputDevice()->addListener(this);

    createProperties();    

    connect(this, SIGNAL(triggerUpdateView()), this, SLOT(updateView()), Qt::QueuedConnection);
}

MidiInCtrl::~MidiInCtrl()
{
    Application::instance()->audioDevicesManager()->midiInputDevice()->removeListener(this);
}

QColor MidiInCtrl::color() const
{
    return cDefaultColor;
}

void MidiInCtrl::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["controller"] = m_pPropController->value();
    data["controllerDefault"] = m_pPropDefaultValue->value();
    AudioUnit::serialize(data, pContext);
}

void MidiInCtrl::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
    m_pPropController->setValue(data["controller"]);
    m_pPropDefaultValue->setValue(data["controllerDefault"]);

    m_controllerValue = m_pPropDefaultValue->value().toInt();
    m_controllerValueNormalized = m_controllerValue / 127.0f;
}

void MidiInCtrl::processStart()
{
    m_controller = cNameToController.values().at(m_pPropController->value().toInt());
    m_controllerValue = m_pPropDefaultValue->value().toInt();
    m_controllerValueNormalized = m_controllerValue / 127.0f;
}

void MidiInCtrl::processStop()
{
}

void MidiInCtrl::process()
{
    m_pOutputValue->setValue(m_controllerValueNormalized);
}

void MidiInCtrl::reset()
{
}

QGraphicsItem* MidiInCtrl::graphicsItem()
{
    if (m_pValueItem == nullptr) {
        m_pValueItem = new QGraphicsSimpleTextItem();
        updateView();
    }
    return m_pValueItem;
}

void MidiInCtrl::inputMidiMessage(const MidiMessage &msg)
{
    if (msg.status() == MidiMessage::Status_ControlChange) {
        if (msg.controllerNumber() == m_controller) {
            m_controllerValue = msg.controllerValue();
            m_controllerValueNormalized = m_controllerValue / 127.0f;
            emit triggerUpdateView();
        }
    }
}

void MidiInCtrl::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropController = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Controller");
    m_pPropController->setAttribute("enumNames", QVariant(cNameToController.keys()));
    m_pPropController->setValue(0);
    QObject::connect(propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty) {
        Q_UNUSED(pProperty);
        QtVariantProperty *pV = dynamic_cast<QtVariantProperty*>(pProperty);
        if (pV == m_pPropController) {
            updateView();
        }
    });

    m_pPropDefaultValue = propertyManager()->addProperty(QVariant::Int, "Default value");
    m_pPropDefaultValue->setAttribute("minimum", 0);
    m_pPropDefaultValue->setAttribute("maximum", 127);
    m_pPropDefaultValue->setValue(100);
    QObject::connect(propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty) {
        if (signalChain() != nullptr && !signalChain()->isStarted()) {
            QtVariantProperty *pV = dynamic_cast<QtVariantProperty*>(pProperty);
            if (pV == m_pPropDefaultValue) {
                m_controllerValue = pV->value().toInt();
                updateView();
            }
        }
    });

    pRoot->addSubProperty(m_pPropController);
    pRoot->addSubProperty(m_pPropDefaultValue);
}

void MidiInCtrl::updateView()
{
    if (m_pValueItem != nullptr) {
        QString text = QString("%1: %2")
                .arg(m_pPropController->valueText())
                .arg(m_controllerValue);
        m_pValueItem->setText(text);
    }
}
