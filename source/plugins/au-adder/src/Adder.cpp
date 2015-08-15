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
#include <QGraphicsPathItem>
#include <QGraphicsSimpleTextItem>
#include "Application.h"
#include "ISignalChain.h"
#include "Adder.h"

Adder::Adder(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputA = addInput("", Signal::Type_Float);
    m_pInputB = addInput("", Signal::Type_Float);
    m_pOutput = addOutput("", Signal::Type_Float);
}

Adder::~Adder()
{
}

void Adder::processStart()
{
}

void Adder::processStop()
{
}

void Adder::process()
{
    float a = m_pInputA->value().asFloat;
    float b = m_pInputB->value().asFloat;
    m_pOutput->setFloatValue(a + b);
}

QGraphicsItem* Adder::graphicsItem()
{
    QGraphicsPathItem *pItem = new QGraphicsPathItem();
    QPainterPath path;
    path.moveTo(0, 0);
    path.lineTo(38, 24);
    path.lineTo(0, 48);
    path.closeSubpath();
    pItem->setPath(path);
    pItem->setBrush(QBrush(QColor(0xFF, 0xFF, 0xE0)));

    QGraphicsSimpleTextItem *pPlus = new QGraphicsSimpleTextItem(pItem);
    pPlus->setText("+");
    QFont font = pPlus->font();
    font.setPixelSize(24);
    pPlus->setFont(font);
    pPlus->setPos(4, 7);

    return pItem;
}

int Adder::flags() const
{
    return Flag_NoTitle | Flag_NoFrame;
}
