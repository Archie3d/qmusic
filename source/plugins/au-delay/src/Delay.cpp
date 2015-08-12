#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "DelayLine.h"
#include "ISignalChain.h"
#include "Delay.h"

Delay::Delay(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInput = addInput("", Signal::Type_Float);
    m_pOutput = addOutput("", Signal::Type_Float);

    m_pDelayLine = nullptr;

    createProperties();
}

Delay::~Delay()
{
    delete m_pDelayLine;
}

void Delay::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["delay"] = m_pPropDelay->value();
    AudioUnit::serialize(data, pContext);
}

void Delay::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropDelay->setValue(data["delay"]);
    AudioUnit::deserialize(data, pContext);
}

void Delay::processStart()
{
    float delayMs = m_pPropDelay->value().toDouble();
    int delaySamples = delayMs / 1000.0 / signalChain()->timeStep();
    m_pDelayLine = new DelayLine(delaySamples);
}

void Delay::processStop()
{
}

void Delay::process()
{
    float out = m_pDelayLine->process(m_pInput->value().asFloat);
    m_pOutput->setFloatValue(out);
}

void Delay::reset()
{
}

void Delay::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropDelay = propertyManager()->addProperty(QVariant::Double, "Delay, ms");
    m_pPropDelay->setValue(100.0);
    m_pPropDelay->setAttribute("minimum", 0.0);
    m_pPropDelay->setAttribute("decimals", 2);
    m_pPropDelay->setAttribute("singleStep", 1.0);
    pRoot->addSubProperty(m_pPropDelay);
}
