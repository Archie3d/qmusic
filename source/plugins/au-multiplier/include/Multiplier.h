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

#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include "AudioUnit.h"

class Multiplier : public AudioUnit
{
public:

    Multiplier(AudioUnitPlugin *pPlugin);
    ~Multiplier();

protected:

    void processStart() override;
    void processStop() override;
    void process() override;
    void reset() override;
    QGraphicsItem* graphicsItem() override;
    int flags() const override;

private:

    void createProperties();

    InputPort *m_pInput;
    InputPort *m_pGain;
    OutputPort *m_pOutput;
};

#endif // MULTIPLIER_H

