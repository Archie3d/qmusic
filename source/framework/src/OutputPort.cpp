#include "SerializationContext.h"
#include "AudioUnit.h"
#include "OutputPort.h"

OutputPort::OutputPort()
    : Port(Direction_Output),
      m_value()
{
}

OutputPort::OutputPort(const QString &name, QVariant::Type type)
    : Port(Direction_Output, name, type),
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

void OutputPort::update()
{
    if (audioUnit() != nullptr) {
        audioUnit()->update();
    }
}

int OutputPort::index() const
{
    AudioUnit *pAu = dynamic_cast<AudioUnit*>(audioUnit());
    Q_ASSERT(pAu != nullptr);

    const QList<OutputPort*> &outputs = pAu->outputs();
    for (int i = 0; i < outputs.count(); ++i) {
        if (outputs[i] == this) {
            return i;
        }
    }
    return -1;
}
