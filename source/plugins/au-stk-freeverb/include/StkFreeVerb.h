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

#ifndef AU_STK_FREEVERB_H
#define AU_STK_FREEVERB_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class FreeVerb;
}

class StkFreeVerb : public AudioUnit
{
public:

    StkFreeVerb(AudioUnitPlugin *pPlugin);
    ~StkFreeVerb();

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

    InputPort *m_pInputLeft;
    InputPort *m_pInputRight;

    OutputPort *m_pOutputLeft;
    OutputPort *m_pOutputRight;

    QtVariantProperty *m_pPropRoomSize;     // 0..1
    QtVariantProperty *m_pPropDamping;      // 0..1
    QtVariantProperty *m_pPropWidth;        // 0..1 (left-rigt mixing)
    QtVariantProperty *m_pPropFrozen;       // t/f
    QtVariantProperty *m_pPropEffectMix;    // 0..1

    stk::FreeVerb *m_pFreeVerb;
};

#endif // AU_STK_FREEVERB_H
