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

#ifndef AU_BIQUAD_H
#define AU_BIQUAD_H

#include "BiquadFilter.h"
#include "AudioUnit.h"

class QtVariantProperty;

class BiQuadFilterUnit : public AudioUnit
{
public:

    enum Type {
        Type_LPF,       ///< Low-pass.
        Type_HPF,       ///< High-pass.
        Type_BPF,       ///< Band-pass.
        Type_Notch,     ///< Notch.
        Type_APF,       ///< All-pass
        Type_PeakingEQ, ///< Peaking equalizer filter.
        Type_LowShelf,  ///< Low-pass shelf.
        Type_HighShelf  ///< High-pass shelf.
    };

    BiQuadFilterUnit(AudioUnitPlugin *pPlugin);
    ~BiQuadFilterUnit();

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

    BiquadFilter m_filter;
    Type m_filterType;
    float m_f;

    InputPort *m_pInput;
    InputPort *m_pInputCutOffFreq;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pFilterType;
    QtVariantProperty *m_pQFactor;
};

#endif // AU_BIQUAD_H

