#include <QDebug>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Generator.h"

const QColor cDefaultColor(140, 200, 180);

// Naive sawtooth generator implementation
double sawtooth(double phase)
{
    return 2.0 * phase - 1.0;
}

// Bandpass-limited sawtooth using Fourier series
double bpl_sawtooth(double phase, double delta)
{
    int n = 0.5 / delta;
    double k = M_PI / 2.0 / n;

    double x = phase * 2.0 * M_PI;
    double o = 0.0;
    for (int i = 1; i <= n; i++) {
        // Limit harmonics amplitude to prevent overflow
        double a = cos((i - 1) * k);
        o += a * a * (1.0 / double(i)) * sin(double(i) * x);
    }
    return -o;
}

// Native triangle waveform generator
double triangle(double phase)
{
    return 2.0 * fabs(sawtooth(phase)) - 1.0;
}

// Bandpass limited triangle waveform using Fourier series
double bpl_triangle(double phase, double delta)
{
    int n = 0.25 / delta;
    double k = M_PI / 2.0 / n;

    double x = phase * 2.0 * M_PI;
    double o = 0.0;
    for (int i = 0; i < n; i++) {
        double a = cos(i * k);
        double d = double(2*i + 1);
        double h = (i % 2 == 0) ? 1.0 : -1.0;
        o += a*a* h * sin(d * x) / d / d;
    }
    o *= 8 / M_PI / M_PI;
    return o;
}

// Naive square waveform generator
double square(double phase)
{
    return phase - 0.5 > 0.0 ? 1.0 : -1.0;
}

// Bandpass limited square waveform using Fourier series
double bpl_square(double phase, double delta)
{
    int n = 0.25 / delta;
    double k = M_PI / 2.0 / n;

    double x = phase * 2.0 * M_PI;
    double o = 0.0;
    for (int i = 1; i <= n; i+=2) {
        // Limit harmonics amplitude to prevent overflow
        double a = cos((i - 1) * k);
        o += a * a * (1 / double(i)) * sin(double(i) * x);
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
    data["BandPassLimit"] = m_pPropBandPassLimit->value();
    AudioUnit::serialize(data, pContext);
}

void Generator::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropWaveform->setValue(data["waveform"]);
    m_pPropBandPassLimit->setValue(data["BandPassLimit"]);
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
    bool bpLimit = m_pPropBandPassLimit->value().toBool();

    double out = 0.0;
    switch (waveform) {
    case 0:
        out = sin(m_phase * 2 * M_PI);
        break;
    case 1:
        out = bpLimit ? bpl_sawtooth(m_phase, dPhase) : sawtooth(m_phase);
        break;
    case 2:
        out = bpLimit ? bpl_square(m_phase, dPhase) : square(m_phase);
        break;
    case 3:
        out = bpLimit ? bpl_triangle(m_phase, dPhase) : triangle(m_phase);
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
}

void Generator::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropWaveform = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Waveform");
    QVariantList list;
    list << "Sine" << "Sawtooth" << "Square" << "Triangle";
    m_pPropWaveform->setAttribute("enumNames", list);
    m_pPropWaveform->setValue(0);

    m_pPropBandPassLimit = propertyManager()->addProperty(QVariant::Bool, "Limit bandpass");
    m_pPropBandPassLimit->setValue(false);

    pRoot->addSubProperty(m_pPropWaveform);
    pRoot->addSubProperty(m_pPropBandPassLimit);
}
