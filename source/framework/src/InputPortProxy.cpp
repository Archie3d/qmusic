#include "InputPortProxy.h"

InputPortProxy::InputPortProxy()
    : InputPort(),
      m_pInputPort(nullptr)
{
}

InputPortProxy::InputPortProxy(InputPort *pInput)
    : InputPort(pInput->name(), pInput->dataType()),
      m_pInputPort(pInput)
{
}

InputPortProxy::InputPortProxy(const QString &name, InputPort *pInput)
    : InputPort(name, pInput->dataType()),
      m_pInputPort(pInput)
{
}

Signal InputPortProxy::value() const
{
    return m_pInputPort == nullptr ? Signal() : m_pInputPort->value();
}

void InputPortProxy::update()
{
    if (m_pInputPort != nullptr) {
        m_pInputPort->update();
    }
}
