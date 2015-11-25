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

#ifndef CONTROLLEREVENT_H
#define CONTROLLEREVENT_H

#include "FrameworkApi.h"
#include "SignalChainEvent.h"

/**
 * @brief Controller change event.
 *
 * This class represents a MIDI controller value change event.
 * A controler is identified by its number.
 * Controller's value is limited (in MIDI) to [0..127] range.
 */
class QMUSIC_FRAMEWORK_API ControllerEvent : public SignalChainEvent
{
public:

    /**
     * Construct controller event.
     * @param number Controller number.
     * @param value Controller value.
     */
    ControllerEvent(int number = 0, int value = 0);

    /**
     * Copy constructor.
     * @param evt
     */
    ControllerEvent(const ControllerEvent &evt);

    /**
     * Assignment operator.
     * @param evt
     * @return
     */
    ControllerEvent& operator =(const ControllerEvent &evt);

    // from SignalChainEvent
    SignalChainEvent* clone() const;
    QString toString() const;

    int controlNumber() const { return m_number; }
    int controlValue() const { return m_value; }

    /**
     * Returns controller value within [0..1] range as
     * a floating point number.
     * @return
     */
    float normalizedControlValue() const;

private:
    int m_number;   ///< Controller number.
    int m_value;    ///< Controller value.
};

#endif // CONTROLLEREVENT_H

