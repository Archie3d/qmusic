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

#ifndef INPUTPORTPROXY_H
#define INPUTPORTPROXY_H

#include "FrameworkApi.h"
#include "InputPort.h"

/**
 * Input port that references another input port.
 */
class QMUSIC_FRAMEWORK_API InputPortProxy : public InputPort
{
public:
    InputPortProxy();
    InputPortProxy(InputPort *pInput);
    InputPortProxy(const QString &name, InputPort *pInput);

    Signal value() const override;
    void update() override;
private:

    /// Associated input port.
    InputPort *m_pInputPort;
};

#endif // INPUTPORTPROXY_H

