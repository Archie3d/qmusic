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
        InputPort *pInput = addInput("", Signal::Type_Float);
        m_inputs.append(pInput);
    }
    m_pOutput = addOutput("", Signal::Type_Float);
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
    float sum = 0.0;
    foreach (const InputPort *pInput, m_inputs) {
        sum += pInput->value().asFloat;
    }

    m_pOutput->setFloatValue(sum);
}
