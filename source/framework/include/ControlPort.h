#ifndef CONTROLPORT_H
#define CONTROLPORT_H

#include "FrameworkApi.h"
#include "Port.h"

class QMUSIC_FRAMEWORK_API ControlPort : public Port
{
public:

    ControlPort(Direction dir = Direction_Input,
                const QString &name = QString(),
                const QVariant::Type type = QVariant::Invalid);

    virtual void setValue(const QVariant &value) = 0;

    void update() override;
};

#endif // CONTROLPORT_H

