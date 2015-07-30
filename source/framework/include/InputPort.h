#ifndef INPUTPORT_H
#define INPUTPORT_H

#include <QVariant>
#include "Port.h"
#include "FrameworkApi.h"

class OutputPort;

class QMUSIC_FRAMEWORK_API InputPort : public Port
{
public:

    const static QString UID;

    InputPort();
    InputPort(const QString &name, QVariant::Type type);

    QVariant value() const override;
    void update() override;

    void connect(OutputPort *pOutput);
    void disconnect();

    OutputPort* connectedOutputPort() const { return m_pConnectedOutputPort; }

    // ISerializable interface
    QString uid() const { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new InputPort(); }

private:

    /// Pointer to connected output port, if any.
    OutputPort *m_pConnectedOutputPort;
};

#endif // INPUTPORT_H

