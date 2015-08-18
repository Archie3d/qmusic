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

#ifndef AU_STK_BEETHREE_H
#define AU_STK_BEETHREE_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class BeeThree;
}

class StkBeeThree : public AudioUnit
{
public:

    enum {
        Ctrl_Operator4 = 2,
        Ctrl_Operator3 = 4,
        Ctrl_LFOSpeed = 11,
        Ctrl_LFODepth = 1
    };


    StkBeeThree(AudioUnitPlugin *pPlugin);
    ~StkBeeThree();

    void handleEvent(SignalChainEvent *pEvent) override;

    QColor color() const override { return QColor(250, 240, 255); }

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

    InputPort *m_pInputFreq;
    InputPort *m_pInputVelocity;

    OutputPort *m_pOutput;

    int m_note;

    QtVariantProperty *m_pPropOperator4;
    QtVariantProperty *m_pPropOperator3;
    QtVariantProperty *m_pPropLFOSpeed;
    QtVariantProperty *m_pPropLFODepth;

    stk::BeeThree *m_pBeeThree;
};

#endif // AU_STK_BEETHREE_H

