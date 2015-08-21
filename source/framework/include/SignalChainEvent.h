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

    /// Event types
    enum Type {
        Invalid = 0,
        NoteOn = 1,
        NoteOff = 2,
        PitchBend = 3,
        Controller = 4
    };

    /**
     * Construct an event.
     * @param name Event name.
     */
    SignalChainEvent(Type type = Invalid);
    SignalChainEvent(const SignalChainEvent &evt);
    SignalChainEvent& operator =(const SignalChainEvent &evt);
    virtual ~SignalChainEvent() {}

    /**
     * Clone this event.
     * This will create an exact copy of this event object.
     * @return
     */
    virtual SignalChainEvent* clone() const = 0;

    /**
     * Returns the event type.
     * @return Event type.
     */
    Type type() const { return m_type; }

    /**
     * Returns textual (string) representation of this event.
     * @return
     */
    virtual QString toString() const;

protected:

    /**
     * Returns textual representation of the event type.
     * @param type
     * @return
     */
    static QString eventTypeToString(Type type);

private:

    Type m_type;     ///< Event name.
};

#endif // SIGNALCHAINEVENT_H

