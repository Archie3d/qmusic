#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <Guitar.h>
#include "Application.h"
#include "ISignalChain.h"
#include "StkGuitar.h"

const float cMinFrequency(50.0f);

void setCtrlPropertyAttrs(QtVariantProperty *pProp, double v = 0.5, double min = 0.0, double max = 1.0)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", min);
    pProp->setAttribute("maximum", max);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(v);
}

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
    data["pickPosition"] = m_pPropPickPosition->value();
    data["stringDumping"] = m_pPropStringDumping->value();
    AudioUnit::serialize(data, pContext);
}

void StkGuitar::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropPickPosition->setValue(data["pickPosition"]);
    m_pPropStringDumping->setValue(data["stringDumping"]);
    AudioUnit::deserialize(data, pContext);
}

void StkGuitar::processStart()
{
    m_pGuitar->setSampleRate(signalChain()->sampleRate());
    m_noteOn = false;
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

    m_pGuitar->controlChange(Ctrl_PickPosition, 128.0 * m_pPropPickPosition->value().toDouble());
    m_pGuitar->controlChange(Ctrl_StringDumping, 128.0 * m_pPropStringDumping->value().toDouble());

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

    m_pPropPickPosition = propertyManager()->addProperty(QVariant::Double, "Pick position");
    setCtrlPropertyAttrs(m_pPropPickPosition, 0.5, 0.01, 1.0);

    m_pPropStringDumping = propertyManager()->addProperty(QVariant::Double, "String dumping");
    setCtrlPropertyAttrs(m_pPropStringDumping, 0.5, 0.0, 0.99);


    pRoot->addSubProperty(m_pPropPickPosition);
    pRoot->addSubProperty(m_pPropStringDumping);
}
