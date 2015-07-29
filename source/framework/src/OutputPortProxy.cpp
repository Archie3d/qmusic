#include "../include/OutputPortProxy.h"

OutputPortProxy::OutputPortProxy()
    : OutputPort(),
      m_outputPtr(nullptr)
{
}

OutputPortProxy::OutputPortProxy(const OutputPortPtr &output)
    : OutputPort(output->name(), output->dataType()),
      m_outputPtr(output)
{
}

OutputPortProxy::OutputPortProxy(const QString &name, const OutputPortPtr &output)
    : OutputPort(name, output->dataType()),
      m_outputPtr(output)
{
}

QVariant OutputPortProxy::value() const
{
    return m_outputPtr.isNull() ? QVariant() : m_outputPtr->value();
}

void OutputPortProxy::update()
{
    if (!m_outputPtr.isNull()) {
        update();
    }
}
