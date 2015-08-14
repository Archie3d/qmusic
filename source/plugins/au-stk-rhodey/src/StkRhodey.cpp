#include <QtVariantPropertyManager>
#include <QDebug>
#include <QtVariantProperty>
#include <Rhodey.h>
#include "Application.h"
#include "ISignalChain.h"
#include "StkRhodey.h"

StkRhodey::StkRhodey(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f", Signal::Type_Float);
    m_pInputVelocity = addInput("amp", Signal::Type_Float);
    m_pInputNoteOn = addInput("on", Signal::Type_Bool);

    m_pOutput = addOutput("out", Signal::Type_Float);

    m_noteOn = false;

    createProperties();

    QString rawPath = QApplication::applicationDirPath() + "/rawwaves";
    stk::Stk::setRawwavePath(rawPath.toStdString());
    m_pRhodey = nullptr;
    try {
        m_pRhodey = new stk::Rhodey();
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkRhodey::~StkRhodey()
{
    delete m_pRhodey;
}

void StkRhodey::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
}

void StkRhodey::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
}

void StkRhodey::processStart()
{
    if (m_pRhodey != nullptr) {
        m_pRhodey->setSampleRate(signalChain()->sampleRate());
    }
    m_noteOn = false;
}

void StkRhodey::processStop()
{
    reset();
}

void StkRhodey::process()
{
    if (m_pRhodey == nullptr) {
        return;
    }

    bool noteOn = m_pInputNoteOn->value().asBool;
    float freq = m_pInputFreq->value().asFloat;
    float amp = m_pInputVelocity->value().asFloat;

    if (noteOn && !m_noteOn) {
        // Note goes on
        m_pRhodey->noteOn(freq, amp);
    } else if (!noteOn && m_noteOn) {
        // Note goes off
        m_pRhodey->noteOff(amp);
    } else {
        m_pRhodey->setFrequency(freq);
    }
    m_noteOn = noteOn;

    float sample = m_pRhodey->tick();

    m_pOutput->setFloatValue(sample);
}

void StkRhodey::reset()
{
    m_noteOn = false;
}

void StkRhodey::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
}
