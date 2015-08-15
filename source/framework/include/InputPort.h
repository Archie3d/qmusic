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

#ifndef INPUTPORT_H
#define INPUTPORT_H

#include "Port.h"
#include "FrameworkApi.h"

class OutputPort;

/**
 * @brief Input port.
 *
 * Incoming signal port.
 * Input port holds a pointer to connected output port (if any).
 * The data (value) provided by the input port comed from
 * connected output port.
 *
 * Input port does not perform any caching of sugnal data.
 */
class QMUSIC_FRAMEWORK_API InputPort : public Port
{
public:

    /// Default constructor.
    InputPort();

    /**
     * Construct an input port.
     * @param name Port name.
     * @param type Port data type.
     */
    InputPort(const QString &name, Signal::Type type);

    /**
     * Returns port value.
     * Value is taked from connected output port. If not connected an invalid value is returned.
     * @return
     */
    Signal value() const override;

    /**
     * Update the input.
     * This will trigger update of connected output port (if any).
     */
    void update() override;

    /**
     * Returns this input port index (within the list of all inut ports of this audio unit).
     * @return
     */
    int index() const override final;

    /**
     * Connect to an output port.
     * @param pOutput Pointer to the output port to connect to.
     */
    void connect(OutputPort *pOutput);

    /**
     * Disconnect this port from the output.
     */
    void disconnect();

    /**
     * Returns pointer to connected outpu port.
     * @return
     */
    OutputPort* connectedOutputPort() const { return m_pConnectedOutputPort; }

private:

    /// Pointer to connected output port, if any.
    OutputPort *m_pConnectedOutputPort;
};

#endif // INPUTPORT_H

