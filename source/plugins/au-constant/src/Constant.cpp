#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QGraphicsWidget>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Constant.h"

Constant::Constant(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutput = addOutput("", QVariant::Double);
    createProperties();

    m_pValueItem = nullptr;
}

Constant::~Constant()
{
}

void Constant::processStart()
{
}

void Constant::processStop()
{
}

void Constant::process()
{
    m_pOutput->setValue(m_pPropConstant->value());
}

void Constant::reset()
{
}

void Constant::control(const QString &name, const QVariant &value)
{
}

QGraphicsItem* Constant::graphicsItem()
{
    if (m_pValueItem == nullptr) {
        m_pValueItem = new QGraphicsSimpleTextItem();
        m_pValueItem->setText(m_pPropConstant->valueText());
    }
    return m_pValueItem;
}

int Constant::flags() const
{
    return Flag_NoTitle;
}

void Constant::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
    m_pPropConstant = propertyManager()->addProperty(QVariant::Double, "Value");
    m_pPropConstant->setValue(0.0);
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        QtVariantProperty *pV = dynamic_cast<QtVariantProperty*>(pProperty);
        if (pV == m_pPropConstant) {
            if (m_pValueItem != nullptr) {
                m_pValueItem->setText(pV->valueText());
            }
        }
    });
    pRoot->addSubProperty(m_pPropConstant);
}
