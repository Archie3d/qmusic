#ifndef OUTPUTPORTPROXY_H
#define OUTPUTPORTPROXY_H

#include "FrameworkApi.h"
#include "OutputPort.h"

class QMUSIC_FRAMEWORK_API OutputPortProxy : public OutputPort
{
public:

    OutputPortProxy();
    OutputPortProxy(const OutputPortPtr &output);
    OutputPortProxy(const QString &name, const OutputPortPtr &output);

    QVariant value() const override;
    void update() override;

private:

    /// Related output port.
    OutputPortPtr m_outputPtr;
};

typedef QSharedPointer<OutputPortProxy> OutputPortProxyPtr;

#endif // OUTPUTPORTPROXY_H

