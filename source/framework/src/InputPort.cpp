#include "SerializationContext.h"
#include "AudioUnit.h"
#include "OutputPort.h"
#include "InputPort.h"

InputPort::InputPort()
    : Port(Direction_Input),
      m_pConnectedOutputPort(nullptr)
{
}

InputPort::InputPort(const QString &name, QVariant::Type type)
    : Port(Direction_Input, name, type),
      m_pConnectedOutputPort(nullptr)
{
}

QVariant InputPort::value() const
{
    return m_pConnectedOutputPort == nullptr ? QVariant() : m_pConnectedOutputPort->value();
}

void InputPort::update()
{
    if (m_pConnectedOutputPort != nullptr) {
        m_pConnectedOutputPort->update();
    }
}

int InputPort::index() const
{
    AudioUnit *pAu = dynamic_cast<AudioUnit*>(audioUnit());
    Q_ASSERT(pAu != nullptr);

    const QList<InputPort*> &inputs = pAu->inputs();
    for (int i = 0; i < inputs.count(); ++i) {
        if (inputs[i] == this) {
            return i;
        }
    }
    return -1;
}

void InputPort::connect(OutputPort *pOutput)
{
    m_pConnectedOutputPort = pOutput;
}

void InputPort::disconnect()
{
    m_pConnectedOutputPort = nullptr;
}
