#include "OutputPort.h"
#include "InputPort.h"

InputPort::InputPort()
    : Port(Direction_Input),
      m_pConnectedOutputPort(nullptr)
{
}

InputPort::InputPort(const QString &name, QVariant::Type type)
    : Port(Direction_Input, name, type)
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

void InputPort::connect(OutputPort *pOutput)
{
    m_pConnectedOutputPort = pOutput;
}

void InputPort::disconnect()
{
    m_pConnectedOutputPort = nullptr;
}
