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

#ifndef AU_STK_BOWED_H
#define AU_STK_BOWED_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Bowed;
}

class StkBowed : public AudioUnit
{
public:

    // Control messages
    enum {
        Ctrl_BowPressure = 2,
        Ctrl_BowPosition = 4,
        Ctrl_VibratoFrequency = 11,
        Ctrl_VibratoGain = 1,
        Ctrl_BowVelocity = 100,
        Ctrl_Volume = 128
    };

    StkBowed(AudioUnitPlugin *pPlugin);
    ~StkBowed();

    QColor color() const override { return QColor(250, 240, 255); }

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    void noteOnEvent(NoteOnEvent *pEvent);
    void noteOffEvent(NoteOffEvent *pEvent);

private:

    void createProperties();
    void setProperties();

    InputPort *m_pInputFreq;
    InputPort *m_pInputVelocity;

    OutputPort *m_pOutput;

    int m_note;

    QtVariantProperty *m_pPropBowPressure;
    QtVariantProperty *m_pPropBowPosition;
    QtVariantProperty *m_pPropVibratoFrequency;
    QtVariantProperty *m_pPropVibratoGain;

    stk::Bowed *m_pBowed;
};

#endif // AU_STK_BOWED_H

