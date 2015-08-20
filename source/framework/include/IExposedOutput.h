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

#ifndef IEXPOSEDOUTPUT_H
#define IEXPOSEDOUTPUT_H

#include "FrameworkApi.h"
#include "OutputPort.h"

/**
 * @brief Exposed output port audio unit interface.
 *
 * Interface to an audio unit that acts as an exposed output port.
 */
class QMUSIC_FRAMEWORK_API IExposedOutput
{
public:

    virtual QString exposedOutputName() const = 0;

    /**
     * Assign referenced output port that will be used to dump the values to.
     * @note The values will be added to the existing value in the referenced port.
     * @param pInputPort
     */
    virtual void setRefOutputPort(OutputPort *pOutputPort) = 0;

    virtual ~IExposedOutput();
};

#endif // IEXPOSEDOUTPUT_H

