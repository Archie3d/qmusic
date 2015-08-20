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

#ifndef IEXPOSEDINPUT_H
#define IEXPOSEDINPUT_H

#include "FrameworkApi.h"
#include "InputPort.h"

/**
 * @brief Exposed input port audio unit interface.
 *
 * Interface to an audio unit that acts as an exposed input port.
 */
class QMUSIC_FRAMEWORK_API IExposedInput
{
public:

    virtual QString exposedInputName() const = 0;

    /**
     * Assign referenced input port that will be used to fetch the values from.
     * @param pInputPort
     */
    virtual void setRefInputPort(InputPort *pInputPort) = 0;

    virtual ~IExposedInput();
};

#endif // IEXPOSEDINPUT_H

