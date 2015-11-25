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

#ifndef PORT_H
#define PORT_H

#include <QString>
#include "FrameworkApi.h"
#include "Signal.h"

class IAudioUnit;

/**
 * @brief Abstract implementation of a signal port.
 *
 * This class is further specialized into InputPort and OutputPort.
 */
class QMUSIC_FRAMEWORK_API Port
{
public:

    /// Port data flow direction.
    enum Direction {
        Direction_Input,    ///< Input port.
        Direction_Output    ///< Output port
    };

    /**
     * @brief Construct a port.
     * @param dir Data flow direction.
     * @param name Port name.
     */
    Port(Direction dir = Direction_Input,
         const QString &name = QString());

    virtual ~Port() {}

    Direction direction() const { return m_direction; }
    QString name() const { return m_name; }

    /**
     * Returns index of this port within the set of audio unit ports.
     * @return
     */
    virtual int index() const = 0;

    /**
     * Assign audio unit this port belongs to.
     * @param pAudioUnit
     */
    void setAudioUnit(IAudioUnit *pAudioUnit) { m_pAudioUnit = pAudioUnit; }

    /**
     * Returns audio unit this port belongs to.
     * @return
     */
    IAudioUnit* audioUnit() const { return m_pAudioUnit; }

    /**
     * @brief Returns value currently set on this port.
     * @return Signal data value.
     */
    virtual float value() const = 0;

    /**
     * @brief Update signal chain.
     */
    virtual void update() = 0;

private:

    Direction m_direction;      ///< Data flow direction.
    QString m_name;             ///< Port name.

    /// Pointer to the audio unit this port belongs to.
    IAudioUnit *m_pAudioUnit;
};

#endif // PORT_H

