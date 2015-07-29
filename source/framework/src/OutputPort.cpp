#include "../include/OutputPort.h"

OutputPort::OutputPort()
    : Port(Direction_Output),
      m_pAudioUnit(nullptr),
      m_value()
{
}

OutputPort::OutputPort(const QString &name, QVariant::Type type)
    : Port(Direction_Output, name, type),
      m_pAudioUnit(nullptr),
      m_value()
{
}

void OutputPort::setValue(const QVariant &value)
{
    if (value.canConvert(dataType())) {
        m_value = value;
        m_value.convert(dataType());
    }
}

void OutputPort::setAudioUnit(IAudioUnit *pAudioUnit)
{
    m_pAudioUnit = pAudioUnit;
}

void OutputPort::update()
{
    if (m_pAudioUnit != nullptr) {
        m_pAudioUnit->update();
    }
}
