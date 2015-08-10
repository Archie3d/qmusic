#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/GeneratorSine.h"

#define RAD(x) ((x) * M_PI / 180.0);

const QColor cDefaultColor(180, 250, 220);

GeneratorSine::GeneratorSine(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_phase(0.0)
{
    m_pInputFreq = addInput("f", QVariant::Double);
    m_pOutput = addOutput("out", QVariant::Double);

    createProperties();
}

QColor GeneratorSine::color() const
{
    return cDefaultColor;
}

void GeneratorSine::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["frequencyScale"] = m_pPropFreqScale->value();
    data["amplitude"] = m_pPropAmplitude->value();
    data["phase"] = m_pPropPhase->value();
    AudioUnit::serialize(data, pContext);
}

void GeneratorSine::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropFreqScale->setValue(data["frequencyScale"]);
    m_pPropAmplitude->setValue(data["amplitude"]);
    m_pPropPhase->setValue(data["phase"]);
    AudioUnit::deserialize(data, pContext);
}

void GeneratorSine::processStart()
{
    m_phase = m_pPropPhase->value().toDouble();
}

void GeneratorSine::processStop()
{
}

void GeneratorSine::process()
{
    double amp = m_pPropAmplitude->value().toDouble();
    double out = amp * sin(m_phase * 2.0 * M_PI);

    double freqScale = m_pPropFreqScale->value().toDouble();
    ISignalChain* chain = signalChain();
    double dt = chain->timeStep();
    double f = m_pInputFreq->value().toDouble();

    double dPhase = f * freqScale * dt;
    m_phase = fmod(m_phase + dPhase, 1.0);
\
    m_pOutput->setValue(out);
}

void GeneratorSine::reset()
{
    m_phase = RAD(m_pPropPhase->value().toDouble());
}

void GeneratorSine::control(const QString &name, const QVariant &value)
{
}

void GeneratorSine::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropFreqScale = propertyManager()->addProperty(QVariant::Double, "Frequency scale");
    m_pPropFreqScale->setAttribute("minimum", 0.01);
    m_pPropFreqScale->setAttribute("maximum", 100.0);
    m_pPropFreqScale->setAttribute("singleStep", 1.0);
    m_pPropFreqScale->setValue(1.0);

    m_pPropAmplitude = propertyManager()->addProperty(QVariant::Double, "Amplitude");
    m_pPropAmplitude->setAttribute("minimum", 0.0);
    m_pPropAmplitude->setAttribute("maximum", 1.0);
    m_pPropAmplitude->setValue(1.0);

    m_pPropPhase = propertyManager()->addProperty(QVariant::Double, "Phase, \302\260");
    m_pPropPhase->setAttribute("minumum", -180.0);
    m_pPropPhase->setAttribute("maximum", 180);
    m_pPropPhase->setValue(0.0);


    pRoot->addSubProperty(m_pPropFreqScale);
    pRoot->addSubProperty(m_pPropAmplitude);
    pRoot->addSubProperty(m_pPropPhase);
}
