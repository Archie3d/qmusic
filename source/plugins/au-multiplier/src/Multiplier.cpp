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

    m_pInput = addInput("", Signal::Type_Float);
    m_pGain = addInput("", Signal::Type_Float);
    m_pOutput = addOutput("", Signal::Type_Float);
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
    float v = m_pInput->value().asFloat;
    float gain = m_pGain->value().asFloat;
    m_pOutput->setFloatValue(v * gain);
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

