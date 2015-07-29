#ifndef INPUTPORTPROXY_H
#define INPUTPORTPROXY_H

#include "FrameworkApi.h"
#include "InputPort.h"

class QMUSIC_FRAMEWORK_API InputPortProxy : public InputPort
{
public:
    InputPortProxy();
    InputPortProxy(const InputPortPtr &inputPtr);
    InputPortProxy(const QString &name, const InputPortPtr &inputPtr);

    QVariant value() const override;
    void update() override;
private:

    /// Associated input port.
    InputPortPtr m_inputPtr;
};

typedef QSharedPointer<InputPortProxy> InputPortProxyPtr;

#endif // INPUTPORTPROXY_H

