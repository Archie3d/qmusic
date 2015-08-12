#include "SerializationContext.h"
#include "AudioUnit.h"
#include "OutputPort.h"

OutputPort::OutputPort()
    : Port(Direction_Output),
      m_value()
{
}

OutputPort::OutputPort(const QString &name, Signal::Type type)
    : Port(Direction_Output, name, type),
      m_value()
{
}

void OutputPort::setValue(const Signal &value)
{
    m_value = value;
}

void OutputPort::update()
{
    if (audioUnit() != nullptr) {
        audioUnit()->update();
    }
}

int OutputPort::index() const
{
    AudioUnit *pAu = static_cast<AudioUnit*>(audioUnit());

    const QList<OutputPort*> &outputs = pAu->outputs();
    for (int i = 0; i < outputs.count(); ++i) {
        if (outputs[i] == this) {
            return i;
        }
    }
    return -1;
}
