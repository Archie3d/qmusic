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
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "Multiplier.h"

const float cDefaultGain = 1.0f;

Multiplier::Multiplier(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{

    m_pInput = addInput();
    m_pGain = addInput();
    m_pOutput = addOutput();
}

Multiplier::~Multiplier()
{
}

void Multiplier::processStart()
{
}

void Multiplier::processStop()
{
}

void Multiplier::process()
{
    float v = m_pInput->value();
    float gain = m_pGain->value();
    m_pOutput->setValue(v * gain);
}

void Multiplier::reset()
{
}

QGraphicsItem* Multiplier::graphicsItem()
{
    QGraphicsPathItem *pItem = new QGraphicsPathItem();
    QPainterPath path;

    path.addEllipse(0, 4, 32, 32);

    pItem->setPath(path);
    pItem->setBrush(QBrush(QColor(0xFF, 0xFF, 0xE0)));

    QGraphicsSimpleTextItem *pPlus = new QGraphicsSimpleTextItem(pItem);
    pPlus->setText(QChar(0xD7));
    QFont font = pPlus->font();
    font.setPixelSize(24);
    pPlus->setFont(font);
    pPlus->setPos(7, 3);

    return pItem;

}

int Multiplier::flags() const
{
    return Flag_NoTitle | Flag_NoFrame;
}

