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

