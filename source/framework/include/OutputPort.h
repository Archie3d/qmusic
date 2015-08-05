#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include <QSharedPointer>
#include "FrameworkApi.h"
#include "IAudioUnit.h"
#include "Port.h"

class QMUSIC_FRAMEWORK_API OutputPort : public Port
{
public:

    OutputPort();
    OutputPort(const QString &name, QVariant::Type type);

    QVariant value() const override { return m_value; }
    void setValue(const QVariant &value);

    void update() override;

    int index() const override final;
private:

    /// Value stored in this output port.
    QVariant m_value;
};

#endif // OUTPUTPORT_H

