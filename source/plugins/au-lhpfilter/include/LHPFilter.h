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

#ifndef AU_LHPFILTER_H
#define AU_LHPFILTER_H

#include "VAOnePoleFilter.h"
#include "AudioUnit.h"

class QtVariantProperty;


class LHPFilter : public AudioUnit
{
public:

    LHPFilter(AudioUnitPlugin *pPlugin);
    ~LHPFilter();

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

    VAOnePoleFilter m_filter;

    InputPort *m_pInput;
    InputPort *m_pInputCutOffFreq;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pFilterType;
};

#endif // AU_LHPFILTER_H

