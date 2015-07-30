#include "OutputPortProxy.h"

OutputPortProxy::OutputPortProxy()
    : OutputPort(),
      m_pOutputPort(nullptr)
{
}

OutputPortProxy::OutputPortProxy(OutputPort *pOutput)
    : OutputPort(pOutput->name(), pOutput->dataType()),
      m_pOutputPort(pOutput)
{
}

OutputPortProxy::OutputPortProxy(const QString &name, OutputPort *pOutput)
    : OutputPort(name, pOutput->dataType()),
      m_pOutputPort(pOutput)
{
}

QVariant OutputPortProxy::value() const
{
    return m_pOutputPort == nullptr ? QVariant() : m_pOutputPort->value();
}

void OutputPortProxy::update()
{
    if (m_pOutputPort != nullptr) {
        update();
    }
}
