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

#ifndef AU_ADDER_H
#define AU_ADDER_H

#include "AudioUnit.h"

class Adder : public AudioUnit
{
public:

    Adder(AudioUnitPlugin *pPlugin);
    ~Adder();

protected:

    void processStart();
    void processStop();
    void process();
    QGraphicsItem* graphicsItem() override;
    int flags() const override;

private:

    InputPort *m_pInputA;
    InputPort *m_pInputB;
    OutputPort *m_pOutput;
};

#endif // AU_ADDER_H

