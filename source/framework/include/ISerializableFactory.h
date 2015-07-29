#ifndef ISERIALIZABLEFACTORY_H
#define ISERIALIZABLEFACTORY_H

#include "FrameworkApi.h"

class ISerializable;

class QMUSIC_FRAMEWORK_API ISerializableFactory
{
public:

    typedef ISerializable* (*Creator)();

    virtual ISerializable* createObject(const QString &uid) const = 0;
    virtual ~ISerializableFactory() {}
};

#endif // ISERIALIZABLEFACTORY_H

