/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#include <QDebug>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "Generator.h"

const QColor cDefaultColor(140, 200, 180);

const int cMaxHarmonics(32);

/**
 * Polynomial BLEP function.
 * @param t Time (phase), 0 <= t <= 1.0
 * @param dt Time step.
 * @return
 */
double poly_blep(double t, float dt)
{
    // 0 <= t < 1
    if (t < dt) {
        t /= dt;
        return t+t - t*t - 1.0;
    }
    // -1 < t < 0
    else if (t > 1.0 - dt) {
        t = (t - 1.0) / dt;
        return t*t + t+t + 1.0;
    }
    // 0 otherwise
    else return 0.0;
}

// Naive sawtooth generator implementation
float sawtooth(float phase)
{
    return 2.0f * phase - 1.0f;
}

// Poly-BLEP sawtooth
float blep_sawtooth(float phase, float dt)
{
    return sawtooth(phase) - poly_blep(phase, dt);
}

// Bandpass-limited sawtooth using Fourier series
float bpl_sawtooth(float phase, float delta)
{
    int n = 0.5 / delta;
    n = qMin(n, cMaxHarmonics);
    float k = M_PI / 2.0 / n;

    float x = phase * 2.0 * M_PI;
    float o = 0.0;
    for (int i = 1; i <= n; i++) {
        // Limit harmonics amplitude to prevent overflow
        float a = cos((i - 1) * k);
        o += a * a * (1.0 / float(i)) * sin(float(i) * x);
    }
    return -o;
}

// Native triangle waveform generator
float triangle(float phase)
{
    return 2.0f * fabs(sawtooth(phase)) - 1.0f;
}

float blep_triange(float phase, float dPhase, float prev = 0.0f)
{
    float value = -1.0 + (2.0 * phase);
    value = 2.0 * (fabs(value) - 0.5);
    value = dPhase * value + (1 - dPhase) * prev;
    return value;
}

// Bandpass limited triangle waveform using Fourier series
float bpl_triangle(float phase, float delta)
{
    int n = 0.25 / delta;
    n = qMin(n, cMaxHarmonics);
    float k = M_PI / 2.0 / n;

    float x = phase * 2.0 * M_PI;
    float o = 0.0;
    for (int i = 0; i < n; i++) {
        float a = cos(i * k);
        float d = float(2*i + 1);
        float h = (i % 2 == 0) ? 1.0f : -1.0f;
        o += a*a* h * sin(d * x) / d / d;
    }
    o *= float(8.0f / M_PI / M_PI);
    return o;
}

// Naive square waveform generator
float square(float phase)
{
    return phase - 0.5f > 0.0f ? 1.0f : -1.0f;
}

float blep_square(float phase, float dt)
{
    float pwm = 0.5f;
    float value = phase < pwm ? 1.0 : -1.0;
    value += poly_blep(phase, dt);
    value -= poly_blep(fmod(phase + 1.0 - pwm, 1.0), dt);
    return value;
}

// Bandpass limited square waveform using Fourier series
float bpl_square(float phase, float delta)
{
    int n = 0.25 / delta;
    n = qMin(n, 2*cMaxHarmonics);
    float k = M_PI / 2.0 / n;

    float x = phase * 2.0 * M_PI;
    float o = 0.0;
    for (int i = 1; i <= n; i+=2) {
        // Limit harmonics amplitude to prevent overflow
        float a = cos((i - 1) * k);
        o += a * a * (1 / float(i)) * sin(float(i) * x);
    }
    return o;
}

Generator::Generator(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_phase(0.0f)
{
    m_pInputFreq = addInput("f");
    m_pOutput = addOutput();

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
    setValues();
}

void Generator::processStop()
{
    reset();
}

void Generator::process()
{
    ISignalChain* chain = signalChain();
    float dt = chain->timeStep();
    float f = m_pInputFreq->value();
    float dPhase = f * dt;

    float out = 0.0;
    switch (m_waveform) {
    case 0:
        out = sin(m_phase * 2 * M_PI);
        break;
    case 1:
        out = m_bandlimit ? bpl_sawtooth(m_phase, dPhase) : sawtooth(m_phase);
        break;
    case 2:
        out = m_bandlimit ? bpl_square(m_phase, dPhase) : square(m_phase);
        break;
    case 3:
        out = m_bandlimit ? bpl_triangle(m_phase, dPhase) : triangle(m_phase);
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
    m_pOutput->setValue(0.0f);
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

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });
}

void Generator::setValues()
{
    m_waveform = m_pPropWaveform->value().toInt();
    m_bandlimit = m_pPropBandPassLimit->value().toBool();
}
