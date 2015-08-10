#include <QDebug>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Generator.h"


const QColor cDefaultColor(140, 200, 180);

double sawtooth(double phase)
{
    return 2.0 * phase - 1.0;
}

#define N_SAW_HARMONICS 32
double blep_sawtooth(double phase, double delta)
{
    int n = 0.5 / delta;
    n = n > N_SAW_HARMONICS ? N_SAW_HARMONICS : n;
    double x = phase * 2.0 * M_PI;
    double o = 0.0;
    for (int i = 1; i <= n; i++) {
        o += (1.0 / double(i)) * sin(double(i) * x);
    }
    return o;
}

double square(double phase)
{
    return phase - 0.5 > 0.0 ? 1.0 : -1.0;
}

#define N_SQUARE_HARMONICS  16
double blep_square(double phase, double delta)
{
    int n = 0.5 / delta;
    n = n > N_SQUARE_HARMONICS ? N_SQUARE_HARMONICS : n;

    double x = phase * 2.0 * M_PI;
    double o = 0.0;
    for (int i = 1; i < n; i+=2) {
        o += (1 / double(i)) * sin(double(i) * x);
    }
    return o;
}

Generator::Generator(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_phase(0.0)
{
    m_pInputFreq = addInput("f", QVariant::Double);
    m_pOutput = addOutput("out", QVariant::Double);

    createProperties();
}

Generator::~Generator()
{
}

QColor Generator::color() const
{
    return cDefaultColor;
}

void Generator::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["waveform"] = m_pPropWaveform->value();
    AudioUnit::serialize(data, pContext);
}

void Generator::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropWaveform->setValue(data["waveform"]);
    AudioUnit::deserialize(data, pContext);
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
    double f = m_pInputFreq->value().toDouble();
    double dPhase = f * dt;

    int waveform = m_pPropWaveform->value().toInt();
    double out = 0.0;
    switch (waveform) {
    case 0:
        out = sin(m_phase * 2 * M_PI);
        break;
    case 1:
        out = blep_sawtooth(m_phase, dPhase);
        break;
    case 2:
        out = blep_square(m_phase, dPhase);
        break;
    default:
        break;
    }

    m_phase = fmod(m_phase + dPhase, 1.0);

    m_pOutput->setValue(out);
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
}
