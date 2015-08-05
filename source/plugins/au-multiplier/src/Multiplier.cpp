#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "Multiplier.h"

const double cDefaultGain = 1.0;

Multiplier::Multiplier(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{

    m_pInput = addInput("", QVariant::Double);
    m_pGain = addInput("", QVariant::Double);
    m_pOutput = addOutput("", QVariant::Double);
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
    double v = m_pInput->value().toDouble();
    double gain = m_pGain->value().toDouble();
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

