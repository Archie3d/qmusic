#include <QDebug>
#include <QGraphicsPathItem>
#include <QGraphicsSimpleTextItem>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Mixer.h"

Mixer::Mixer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_inputAPtr = InputPortPtr(new InputPort("", QVariant::Double));
    m_inputBPtr = InputPortPtr(new InputPort("", QVariant::Double));
    m_outputPtr = OutputPortPtr(new OutputPort("", QVariant::Double));

    addInput(m_inputAPtr);
    addInput(m_inputBPtr);
    addOutput(m_outputPtr);
}

Mixer::~Mixer()
{
}

void Mixer::processStart()
{
}

void Mixer::processStop()
{
}

void Mixer::process()
{
    double a = m_inputAPtr->value().toDouble();
    double b = m_inputBPtr->value().toDouble();
    m_outputPtr->setValue(a + b);
}

QGraphicsItem* Mixer::graphicsItem()
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

int Mixer::flags() const
{
    return Flag_NoTitle | Flag_NoFrame;
}
