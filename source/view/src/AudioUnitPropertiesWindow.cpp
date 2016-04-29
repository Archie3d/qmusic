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

#include <QtTreePropertyBrowser>
#include <QtVariantEditorFactory>
#include <QtProperty>
#include "AudioUnit.h"
#include "AudioUnitPlugin.h"
#include "AudioUnitPropertiesWindow.h"

AudioUnitPropertiesWindow::AudioUnitPropertiesWindow(QWidget *pParent)
    : QDockWidget(pParent)
{
    setObjectName("audioUnitPropertiesWindow");
    m_pPropertyFactory = new QtVariantEditorFactory(this);
    m_pPropertyBrowser = new QtTreePropertyBrowser();
    setWidget(m_pPropertyBrowser);

    clear();
}

void AudioUnitPropertiesWindow::setAudioUnit(AudioUnit *pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);

    QtVariantPropertyManager *pPropertyManager = pAudioUnit->propertyManager();
    QtVariantProperty *pRootProperty = pAudioUnit->rootProperty();
    QList<QtProperty*> subProperties = pRootProperty->subProperties();

    for (QtProperty *pProperty : subProperties) {
        m_pPropertyBrowser->addProperty(pProperty);
    }

    m_pPropertyBrowser->setFactoryForManager(pPropertyManager, m_pPropertyFactory);
    m_pPropertyBrowser->setPropertiesWithoutValueMarked(true);
    m_pPropertyBrowser->setRootIsDecorated(false);
    m_pPropertyBrowser->setAlternatingRowColors(true);
    m_pPropertyBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);

    setWindowTitle(tr("Properties of '%1'").arg(pAudioUnit->plugin()->name()));
}

void AudioUnitPropertiesWindow::clear()
{
    m_pPropertyBrowser->clear();
    setWindowTitle(tr("Properties"));
}

void AudioUnitPropertiesWindow::handleAudioUnitSelected(AudioUnit *pAudioUnit)
{
    clear();
    if (pAudioUnit == nullptr) {
        return;
    }
    setAudioUnit(pAudioUnit);
}
