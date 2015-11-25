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

#ifndef SIGNALCHAINFACTORY_H
#define SIGNALCHAINFACTORY_H

#include <QMap>
#include "ISerializableFactory.h"
#include "FrameworkApi.h"

/**
 * @brief Factory of signal chain items.
 *
 * This factory is used when deserializing a signel chain from a
 * file or copy-paste buffer.
 */
class QMUSIC_FRAMEWORK_API SignalChainFactory : public ISerializableFactory
{
public:

    SignalChainFactory();

    // ISerializableFactory inrerface
    ISerializable* createObject(const QString &uid) const override;

private:

    /// Register all creators.
    void registerCreators();

    /// List of creators (mapped to serialization UIDs).
    QMap<QString, Creator> m_creators;
};

#endif // SIGNALCHAINFACTORY_H

