#ifndef INPUTPORTPROXY_H
#define INPUTPORTPROXY_H

#include "FrameworkApi.h"
#include "InputPort.h"

class QMUSIC_FRAMEWORK_API InputPortProxy : public InputPort
{
public:
    InputPortProxy();
    InputPortProxy(InputPort *pInput);
    InputPortProxy(const QString &name, InputPort *pInput);

    QVariant value() const override;
    void update() override;
private:

    /// Associated input port.
    InputPort *m_pInputPort;
};

#endif // INPUTPORTPROXY_H

