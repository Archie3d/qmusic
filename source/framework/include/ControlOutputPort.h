#ifndef CONTROLOUTPUTPORT_H
#define CONTROLOUTPUTPORT_H

#include <QList>
#include "FrameworkApi.h"
#include "ControlInputPort.h"

class QMUSIC_FRAMEWORK_API ControlOutputPort : public ControlPort
{
public:
    ControlOutputPort(const QString &name, QVariant::Type type);

    void setValue(const QVariant &value);

    // Output control does not preserve any value
    QVariant value() const { return QVariant(); }

    void connect(const ControlInputPortPtr &input);
    void disconnect(const ControlInputPortPtr &input);
    void disconnectAll();

    QList<ControlInputPortPtr> connectedInputs() const { return m_connectedInputs; }

private:

    QList<ControlInputPortPtr> m_connectedInputs;
};

typedef QSharedPointer<ControlOutputPort> ControlOutputPortPtr;

#endif // CONTROLOUTPUTPORT_H

