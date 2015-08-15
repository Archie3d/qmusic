/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <QVariant>
#include "FrameworkApi.h"

class SerializationContext;

/**
 * @brief Interface to a serializable entity.
 *
 * Any object that can be directly serialized must implement this interface.
 *
 * Serializable class is identified by UID. This UID is used by factory when
 * deserializing the object back.
 *
 * Serialization itself (including recreating of objects via factories)
 * is handled by serialization context.
 */
class QMUSIC_FRAMEWORK_API ISerializable
{
public:

    /**
     * Returns serializable object UID.
     * @return
     */
    virtual QString uid() const = 0;

    /**
     * Serialize this object.
     * @param data Data storage used for serialization.
     * @param pContext Pointer to serialization context.
     */
    virtual void serialize(QVariantMap &data, SerializationContext *pContext) const = 0;

    /**
     * Deserialize Deserialize this object.
     * This method is called immediately after the object is constructed by the factory.
     * It should perform all initialization of the object.
     * @param data Data storage used for deserialization.
     * @param pContext Pointer to serialization context.
     */
    virtual void deserialize(const QVariantMap &data, SerializationContext *pContext) = 0;

    virtual ~ISerializable() {}
};

#endif // ISERIALIZABLE_H

