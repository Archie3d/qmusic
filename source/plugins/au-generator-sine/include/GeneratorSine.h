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

#ifndef AU_GENERATOR_SINE_H
#define AU_GENERATOR_SINE_H

#include "AudioUnit.h"

class QtVariantProperty;

/*
 *  Extended sine wave generator.
 */

class GeneratorSine : public AudioUnit
{
public:

    GeneratorSine(AudioUnitPlugin *pPlugin);

    QColor color() const;

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

    float m_phase;
    float m_amp;
    float m_freqScale;
    float m_dt;

    InputPort *m_pInputFreq;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pPropFreqScale;
    QtVariantProperty *m_pPropAmplitude;
    QtVariantProperty *m_pPropPhase;
};

#endif // AU_GENERATOR_SINE_H

