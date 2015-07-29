#include "../include/InputPortProxy.h"

InputPortProxy::InputPortProxy()
    : InputPort(),
      m_inputPtr(nullptr)
{
}

InputPortProxy::InputPortProxy(const InputPortPtr &inputPtr)
    : InputPort(inputPtr->name(), inputPtr->dataType()),
      m_inputPtr(inputPtr)
{
}

InputPortProxy::InputPortProxy(const QString &name, const InputPortPtr &inputPtr)
    : InputPort(name, inputPtr->dataType()),
      m_inputPtr(inputPtr)
{
}

QVariant InputPortProxy::value() const
{
    return m_inputPtr.isNull() ? QVariant() : m_inputPtr->value();
}

void InputPortProxy::update()
{
    if (m_inputPtr.isNull()) {
        m_inputPtr->update();
    }
}
