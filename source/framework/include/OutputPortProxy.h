#ifndef OUTPUTPORTPROXY_H
#define OUTPUTPORTPROXY_H

#include "FrameworkApi.h"
#include "OutputPort.h"

/**
 * Output port that references another output port.
 */
class QMUSIC_FRAMEWORK_API OutputPortProxy : public OutputPort
{
public:

    OutputPortProxy();
    OutputPortProxy(OutputPort *pOutput);
    OutputPortProxy(const QString &name, OutputPort *pOutput);

    QVariant value() const override;
    void update() override;

private:

    /// Related output port.
    OutputPort *m_pOutputPort;
};

#endif // OUTPUTPORTPROXY_H

