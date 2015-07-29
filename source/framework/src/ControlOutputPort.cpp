#include "ControlOutputPort.h"

ControlOutputPort::ControlOutputPort(const QString &name, QVariant::Type type)
    : ControlPort(Direction_Output, name, type)
{
}

void ControlOutputPort::setValue(const QVariant &value)
{
    foreach (const ControlInputPortPtr &input, m_connectedInputs) {
        input->setValue(value);
    }
}

void ControlOutputPort::connect(const ControlInputPortPtr &input)
{
    m_connectedInputs.append(input);
}

void ControlOutputPort::disconnect(const ControlInputPortPtr &input)
{
    m_connectedInputs.removeOne(input);
}

void ControlOutputPort::disconnectAll()
{
    m_connectedInputs.clear();
}
