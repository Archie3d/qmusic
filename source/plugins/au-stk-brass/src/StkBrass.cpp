#include <QDebug>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <Brass.h>
#include "Application.h"
#include "ISignalChain.h"
#include "StkBrass.h"

const float cLowestFrequency(8.0);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkBrass::StkBrass(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f", Signal::Type_Float);
    m_pInputVelocity = addInput("amp", Signal::Type_Float);
    m_pInputBreath = addInput("breath", Signal::Type_Float);
    m_pInputNoteOn = addInput("on", Signal::Type_Bool);

    m_pOutput = addOutput("out", Signal::Type_Float);

    m_noteOn = false;

    createProperties();

    m_pBrass = nullptr;
    try {
        m_pBrass = new stk::Brass(cLowestFrequency);
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkBrass::~StkBrass()
{
    delete m_pBrass;
}

void StkBrass::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["lipTension"] = m_pPropLipTension->value();
    data["slideLength"] = m_pPropSlideLength->value();
    AudioUnit::serialize(data, pContext);
}

void StkBrass::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropLipTension->setValue(data["lipTension"]);
    m_pPropSlideLength->setValue(data["slideLength"]);
    AudioUnit::deserialize(data, pContext);
}

void StkBrass::processStart()
{
    if (m_pBrass != nullptr) {
        m_pBrass->setSampleRate(signalChain()->sampleRate());
    }
    m_noteOn = false;
}

void StkBrass::processStop()
{
    reset();
}

void StkBrass::process()
{
    if (m_pBrass == nullptr) {
        return;
    }

    m_pBrass->controlChange(Ctrl_LipTension, 128.0 * m_pPropLipTension->value().toDouble());
    m_pBrass->controlChange(Ctrl_SlideLength, 128.0 * m_pPropSlideLength->value().toDouble());

    bool noteOn = m_pInputNoteOn->value().asBool;
    float freq = m_pInputFreq->value().asFloat;
    freq = qMax(freq, 2.0f * cLowestFrequency);
    float amp = m_pInputVelocity->value().asFloat;
    float breath = m_pInputBreath->value().asFloat;

    if (noteOn && !m_noteOn) {
        // Note goes on
        m_pBrass->noteOn(freq, amp);
    } else if (!noteOn && m_noteOn) {
        // Note goes off
        m_pBrass->noteOff(amp);
    } else {
        m_pBrass->setFrequency(freq);
    }
    m_pBrass->controlChange(Ctrl_BreathPressure, 128.0 * breath);

    m_noteOn = noteOn;

    float sample = m_pBrass->tick();

    m_pOutput->setFloatValue(sample);
}

void StkBrass::reset()
{
    m_noteOn = false;
}

void StkBrass::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropLipTension = propertyManager()->addProperty(QVariant::Double, "Lip tension");
    setCtrlPropertyAttrs(m_pPropLipTension);
    m_pPropSlideLength = propertyManager()->addProperty(QVariant::Double, "Slide length");
    setCtrlPropertyAttrs(m_pPropSlideLength);

    pRoot->addSubProperty(m_pPropLipTension);
    pRoot->addSubProperty(m_pPropSlideLength);
}
