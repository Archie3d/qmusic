#include "../include/InputPort.h"

InputPort::InputPort()
    : Port(Direction_Input),
      m_connectedOutputPort(nullptr)
{
}

InputPort::InputPort(const QString &name, QVariant::Type type)
    : Port(Direction_Input, name, type)
{
}

QVariant InputPort::value() const
{
    return m_connectedOutputPort.isNull() ? QVariant() : m_connectedOutputPort->value();
}

void InputPort::update()
{
    if (!m_connectedOutputPort.isNull()) {
        m_connectedOutputPort->update();
    }
}

void InputPort::connect(const OutputPortPtr &outputPtr)
{
    m_connectedOutputPort = outputPtr;
}

void InputPort::disconnect()
{
    m_connectedOutputPort.clear();
}
