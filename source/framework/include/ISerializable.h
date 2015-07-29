#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <QVariant>
#include "FrameworkApi.h"

class SerializationContext;

class QMUSIC_FRAMEWORK_API ISerializable
{
public:

    virtual QString uid() const = 0;
    virtual void serialize(QVariantMap &data, SerializationContext *pContext) const = 0;
    virtual void deserialize(const QVariantMap &data, SerializationContext *pContext) = 0;
    virtual ~ISerializable() {}
};

#endif // ISERIALIZABLE_H

