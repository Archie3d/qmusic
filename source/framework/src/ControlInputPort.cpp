#include "ControlInputPort.h"

ControlInputPort::ControlInputPort(IAudioUnit *pAudioUnit, const QString &name, QVariant::Type type)
    : ControlPort(Direction_Input, name, type),
      m_pAudioUnit(pAudioUnit),
      m_value()
{
    Q_ASSERT(pAudioUnit != nullptr);
}

void ControlInputPort::setValue(const QVariant &value)
{
    Q_ASSERT(m_pAudioUnit != nullptr);
    m_value = value;
    m_pAudioUnit->control(name(), m_value);
}

void ControlInputPort::setAudioUnit(IAudioUnit *pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    m_pAudioUnit = pAudioUnit;
}
