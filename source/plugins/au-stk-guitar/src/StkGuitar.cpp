#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <Guitar.h>
#include "Application.h"
#include "ISignalChain.h"
#include "StkGuitar.h"

const float cMinFrequency(8.0f);

StkGuitar::StkGuitar(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f", Signal::Type_Float);
    m_pInputVelocity = addInput("amp", Signal::Type_Float);
    m_pInputNoteOn = addInput("on", Signal::Type_Bool);

    m_pOutput = addOutput("out", Signal::Type_Float);

    m_noteOn = false;

    createProperties();

    m_pGuitar = new stk::Guitar();
}

StkGuitar::~StkGuitar()
{
    delete m_pGuitar;
}

void StkGuitar::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["pluckPosition"] = m_pPropPluckPosition->value();
    data["loopGain"] = m_pPropLoopGain->value();
    AudioUnit::serialize(data, pContext);
}

void StkGuitar::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropPluckPosition->setValue(data["pluckPosition"]);
    m_pPropLoopGain->setValue(data["loopGain"]);
    AudioUnit::deserialize(data, pContext);
}

void StkGuitar::processStart()
{
    m_pGuitar->setSampleRate(signalChain()->sampleRate());
    m_pGuitar->setPluckPosition(m_pPropPluckPosition->value().toDouble());
    m_pGuitar->setLoopGain(m_pPropLoopGain->value().toDouble());
    m_noteOn = false;
    m_freq = 0.0f;
}

void StkGuitar::processStop()
{
    reset();
}

void StkGuitar::process()
{
    bool noteOn = m_pInputNoteOn->value().asBool;
    float freq = m_pInputFreq->value().asFloat;
    float amp = m_pInputVelocity->value().asFloat;

    if (freq < cMinFrequency) {
        return;
    }

    if (noteOn && !m_noteOn) {
        // Note goes on
        m_pGuitar->noteOn(freq, amp);
    } else if (!noteOn && m_noteOn) {
        // Note goes off
        m_pGuitar->noteOff(amp);
    } else {
        m_pGuitar->setFrequency(freq);
    }
    m_noteOn = noteOn;
    m_freq = freq;

    float sample = m_pGuitar->tick();

    m_pOutput->setFloatValue(sample);
}

void StkGuitar::reset()
{
    m_noteOn = false;
}

void StkGuitar::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropPluckPosition = propertyManager()->addProperty(QVariant::Double, "Pluck position");
    m_pPropPluckPosition->setAttribute("minimum", 0.0);
    m_pPropPluckPosition->setAttribute("maximum", 100.0);
    m_pPropPluckPosition->setAttribute("singleStep", 0.1);
    m_pPropPluckPosition->setValue(0.5);

    m_pPropLoopGain = propertyManager()->addProperty(QVariant::Double, "Loop gain");
    m_pPropLoopGain->setAttribute("minimum", 0.0);
    m_pPropLoopGain->setAttribute("maximum", 1.0);
    m_pPropLoopGain->setValue(0.5);

    pRoot->addSubProperty(m_pPropPluckPosition);
    pRoot->addSubProperty(m_pPropLoopGain);
}
