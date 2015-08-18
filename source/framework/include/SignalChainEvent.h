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

#ifndef SIGNALCHAINEVENT_H
#define SIGNALCHAINEVENT_H

#include <QVariant>
#include "FrameworkApi.h"

/**
 * @brief Signal chain event.
 * Signal chain may produce and consume events. Events are distinguished
 * by their name. Each event can have an arbitrary payload data attached.
 * Signal chain normally dispatches events to its audio units.
 */
class QMUSIC_FRAMEWORK_API SignalChainEvent
{
public:

    /**
     * Construct an event.
     * @param name Event name.
     * @param data Event data.
     */
    SignalChainEvent(const QString &name = QString(),
                     const QVariant &data = QVariant());

    /// Copy-constructor.
    SignalChainEvent(const SignalChainEvent &evt);

    SignalChainEvent& operator =(const SignalChainEvent &evt);

    // Getters & setters

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    const QVariant& data() const { return m_data; }
    void setData(const QVariant &data) { m_data = data; }

private:
    QString m_name;     ///< Event name.
    QVariant m_data;    ///< Event payload data.
};

Q_DECLARE_METATYPE(SignalChainEvent)

QMUSIC_FRAMEWORK_API QDebug operator <<(QDebug dbg, const SignalChainEvent &evt);

#endif // SIGNALCHAINEVENT_H

