#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Generator.h"

const double cDefaultFrequency = 440.0;

double sawtooth(double phase) {
    return (phase - M_PI) / M_PI;
}

double square(double phase) {
    return (phase - M_PI) > 0 ? 1.0 : -1.0;
}

Generator::Generator(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_phase(0.0),
      m_outputPtr(nullptr)
{
    m_inputFreqPtr = addInput("Freq", QVariant::Double);
    m_outputPtr = addOutput("Out", QVariant::Double);

    //addControlInput("Freq", QVariant::Double);
    //addControlOutput("Freq", QVariant::Double);

    createProperties();
}

Generator::~Generator()
{
}

void Generator::processStart()
{
}

void Generator::processStop()
{
}

void Generator::process()
{
    ISignalChain* chain = signalChain();
    double dt = chain->timeStep();
    double f = m_inputFreqPtr->value().toDouble();
    double dPhase = f * 2 * M_PI * dt;
    m_phase += dPhase;
    if (m_phase > 2 * M_PI) {
        m_phase -= 2 * M_PI;
    }

    int waveform = m_pPropWaveform->value().toInt();
    double out = 0.0;
    switch (waveform) {
    case 0:
        out = sin(m_phase);
        break;
    case 1:
        out = sawtooth(m_phase);
        break;
    case 2:
        out = square(m_phase);
        break;
    default:
        break;
    }

    m_outputPtr->setValue(out);
}

void Generator::reset()
{
    m_phase = 0;
}

void Generator::control(const QString &name, const QVariant &value)
{
    if (name == "Freq") {
        m_pPropFrequency->setValue(value);
    }
}

void Generator::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropWaveform = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Waveform");
    QVariantList list;
    list << "Sine" << "Sawtooth" << "Square";
    m_pPropWaveform->setAttribute("enumNames", list);
    m_pPropWaveform->setValue(0);
    pRoot->addSubProperty(m_pPropWaveform);

    /*
    m_pPropFrequency = propertyManager()->addProperty(QVariant::Double, "Frequency");
    m_pPropFrequency->setValue(cDefaultFrequency);
    pRoot->addSubProperty(m_pPropFrequency);
    */
}
