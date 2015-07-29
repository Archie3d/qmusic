#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/Amplifier.h"

const double cDefaultGain = 1.0;

Amplifier::Amplifier(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{

    m_inputPtr = addInput("In", QVariant::Double);
    m_gainPtr = addInput("Gain", QVariant::Double);
    m_outputPtr = addOutput("Out", QVariant::Double);

    createProperties();
}

Amplifier::~Amplifier()
{
}

void Amplifier::processStart()
{
}

void Amplifier::processStop()
{
}

void Amplifier::process()
{
    double v = m_inputPtr->value().toDouble();    
    double gain = m_gainPtr->value().toDouble();
    m_outputPtr->setValue(v * gain);
}

void Amplifier::reset()
{
}

void Amplifier::control(const QString &name, const QVariant &value)
{
}

void Amplifier::createProperties()
{
}
