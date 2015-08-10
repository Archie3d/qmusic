#ifndef ISERIALIZABLEFACTORY_H
#define ISERIALIZABLEFACTORY_H

#include <QString>
#include "FrameworkApi.h"

class ISerializable;

/**
 * @brief Interface of factory of serializable objects.
 */
class QMUSIC_FRAMEWORK_API ISerializableFactory
{
public:

    /// Function to create a serializable object.
    typedef ISerializable* (*Creator)();

    /**
     * Create a new object by its serialization UID.
     * @param uid Object serialization UID.
     * @return Pointer to a new object or null if unable to crreate.
     */
    virtual ISerializable* createObject(const QString &uid) const = 0;

    virtual ~ISerializableFactory() {}
};

#endif // ISERIALIZABLEFACTORY_H

