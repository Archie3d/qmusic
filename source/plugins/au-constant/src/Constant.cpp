#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <QGraphicsWidget>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Constant.h"

const QColor cItemColor(255, 220, 255);

Constant::Constant(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutput = addOutput("", Signal::Type_Float);
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
    m_pOutput->setFloatValue(m_pPropConstant->value().toDouble());
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

QColor Constant::color() const
{
    return cItemColor;
}

int Constant::flags() const
{
    return Flag_NoTitle;
}

void Constant::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["value"] = m_pPropConstant->value();
}

void Constant::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropConstant->setValue(data["value"]);
    AudioUnit::deserialize(data, pContext);
}

void Constant::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
    m_pPropConstant = propertyManager()->addProperty(QVariant::Double, "Value");
    m_pPropConstant->setValue(0.0);
    m_pPropConstant->setAttribute("singleStep", 0.1);

    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        // Update text item with the value for the property.
        QtVariantProperty *pV = dynamic_cast<QtVariantProperty*>(pProperty);
        if (pV == m_pPropConstant) {
            if (m_pValueItem != nullptr) {
                m_pValueItem->setText(pV->valueText());
            }
        }
    });
    pRoot->addSubProperty(m_pPropConstant);
}
