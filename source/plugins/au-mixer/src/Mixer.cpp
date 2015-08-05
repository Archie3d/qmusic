#include <QDebug>
#include <QGraphicsPathItem>
#include <QGraphicsSimpleTextItem>
#include "Application.h"
#include "ISignalChain.h"
#include "Mixer.h"

const int cNumberOfInputs(8);

Mixer::Mixer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    for (int i = 0; i < cNumberOfInputs; ++i) {
        InputPort *pInput = addInput("", QVariant::Double);
        m_inputs.append(pInput);
    }
    m_pOutput = addOutput("", QVariant::Double);
}

Mixer::~Mixer()
{
}

void Mixer::processStart()
{
}

void Mixer::processStop()
{
}

void Mixer::process()
{
    double sum = 0.0;
    foreach (const InputPort *pInput, m_inputs) {
        sum += pInput->value().toDouble();
    }

    m_pOutput->setValue(sum);
}
