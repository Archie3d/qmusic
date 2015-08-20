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

#ifndef OUTPUTPORTPROXY_H
#define OUTPUTPORTPROXY_H

#include "FrameworkApi.h"
#include "OutputPort.h"

/**
 * Output port that references another output port.
 */
class QMUSIC_FRAMEWORK_API OutputPortProxy : public OutputPort
{
public:

    OutputPortProxy();
    OutputPortProxy(OutputPort *pOutput);
    OutputPortProxy(const QString &name, OutputPort *pOutput);

    float value() const override;
    void update() override;

private:

    /// Related output port.
    OutputPort *m_pOutputPort;
};

#endif // OUTPUTPORTPROXY_H

