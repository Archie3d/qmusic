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

#ifndef AU_STK_GUITAR_H
#define AU_STK_GUITAR_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Guitar;
}

class StkGuitar : public AudioUnit
{
public:

    enum {
        Ctrl_CouplingGain = 2,
        Ctrl_PickPosition = 4,
        Ctrl_StringDamping = 11,
        Ctrl_Modulation = 1,
        Ctrl_Aftertouch = 128
    };

    StkGuitar(AudioUnitPlugin *pPlugin);
    ~StkGuitar();

    QColor color() const override { return QColor(250, 240, 255); }

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

protected:

    void processStart() override;
    void processStop() override;
    void process() override;
    void reset() override;

    void noteOnEvent(NoteOnEvent *pEvent) override;
    void noteOffEvent(NoteOffEvent *pEvent) override;

private:

    void createProperties();
    void setValues();

    InputPort *m_pInputFreq;
    InputPort *m_pInputVelocity;

    OutputPort *m_pOutput;

    int m_note;

    QtVariantProperty *m_pPropPickPosition;
    QtVariantProperty *m_pPropStringDamping;

    stk::Guitar *m_pGuitar;
};

#endif // AU_STK_GUITAR_H

