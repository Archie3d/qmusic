#include "SerializationContext.h"
#include "OutputPort.h"
#include "InputPort.h"

const QString InputPort::UID("InputPort");

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

void InputPort::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    Port::serialize(data, pContext);
    data["connectedOutputPort"] = pContext->serialize(m_pConnectedOutputPort);
}

void InputPort::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    Port::deserialize(data, pContext);
    m_pConnectedOutputPort = pContext->deserialize<OutputPort>(data["connectedOutputPort"]);
}
