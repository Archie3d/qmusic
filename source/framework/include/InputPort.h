#ifndef INPUTPORT_H
#define INPUTPORT_H

#include "FrameworkApi.h"
#include "OutputPort.h"

class QMUSIC_FRAMEWORK_API InputPort : public Port
{
public:

    InputPort();
    InputPort(const QString &name, QVariant::Type type);

    QVariant value() const override;
    void update() override;

    void connect(const OutputPortPtr &outputPtr);
    void disconnect();

    OutputPortPtr connectedOutputPort() const { return m_connectedOutputPort; }

private:

    /// Pointer to connected output port, if any.
    OutputPortPtr m_connectedOutputPort;
};

typedef QSharedPointer<InputPort> InputPortPtr;

#endif // INPUTPORT_H

