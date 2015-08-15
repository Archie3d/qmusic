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

#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include "FrameworkApi.h"
#include "IAudioUnit.h"
#include "Port.h"

/**
 * @brief Output port.
 *
 * Outgoing signal port.
 * Output port keeps the value stored by its audio unit.
 * This value is then supplied to all connected input ports.
 */
class QMUSIC_FRAMEWORK_API OutputPort : public Port
{
public:

    /// Default constructor.
    OutputPort();

    /**
     * Construct an output port.
     * @param name Port name.
     * @param type Port data type.
     */
    OutputPort(const QString &name, Signal::Type type);

    /**
     * Returns value stored in this port.
     * @return
     */
    Signal value() const override { return m_value; }

    /**
     * Assign value to this port.
     * @param value
     */
    void setValue(const Signal &value);

    void setIntValue(int v) { m_value.asInt = v; }
    void setFloatValue(float v) { m_value.asFloat = v; }
    void setBoolValue(bool v) { m_value.asBool = v; }

    /**
     * Update the port.
     * This will trigger corresponding audio unit update that eventually
     * will assign the output port value.
     */
    void update() override;

    /**
     * Returns this output port index (within the list of all outputs of the audio unit).
     * @return
     */
    int index() const override final;
private:

    /// Value stored in this output port.
    Signal m_value;
};

#endif // OUTPUTPORT_H

