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
 *
 * @see InputPort
 */
class QMUSIC_FRAMEWORK_API OutputPort : public Port
{

    friend class InputPort;

public:

    /// Default constructor.
    OutputPort();

    /**
     * Construct an output port.
     * @param name Port name.
     */
    OutputPort(const QString &name);

    /**
     * Returns value stored in this port.
     * @return
     */
    float value() const override { return m_value; }

    /**
     * Assign value to this port.
     * @param value
     */
    inline void setValue(float value) { m_value = value; }

    /**
     * Returns port value.
     * @return
     */
    inline float getValue() const { return m_value; }

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

    /**
     * Returns pointer to internal value holder.
     * This is used to speed-up processing and should be used
     * by connected input port only.
     * @return
     */
    const float* valuePtr() const { return &m_value; }


    /// Value stored in this output port.
    float m_value;
};

#endif // OUTPUTPORT_H

