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

#ifndef AU_STK_CUBIC_H
#define AU_STK_CUBIC_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Cubic;
}

/**
 * Cubic distortion
 */
class StkCubic : public AudioUnit
{
public:

    StkCubic(AudioUnitPlugin *pPlugin);
    ~StkCubic();

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

private:

    void createProperties();
    void setValues();

    InputPort *m_pInput;

    OutputPort *m_pOutput;

    QtVariantProperty *m_pPropA1;
    QtVariantProperty *m_pPropA2;
    QtVariantProperty *m_pPropA3;
    QtVariantProperty *m_pPropThreshold;

    stk::Cubic *m_pCubic;
};

#endif // AU_STK_CUBIC_H
