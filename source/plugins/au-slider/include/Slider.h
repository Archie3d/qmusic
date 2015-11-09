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

#ifndef AU_SLIDER_H
#define AU_SLIDER_H

#include "AudioUnit.h"

class QtVariantProperty;
class QSlider;

/**
 * Slider plugin defines a horizontal or vertical slider.
 * A slider output a single value determined by its position.
 */
class Slider : public AudioUnit
{
public:

    Slider(AudioUnitPlugin *pPlugin);
    ~Slider();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    QGraphicsItem* graphicsItem();
    int flags() const;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

private:

    void createProperties();
    void updateSliderValues();
    void updateSliderOrientation();
    void onSliderValueChanged(int pos);

    OutputPort *m_pOutput;
    QtVariantProperty *m_pPropLabel;
    QtVariantProperty *m_pPropValue;
    QtVariantProperty *m_pPropMin;
    QtVariantProperty *m_pPropMax;
    QtVariantProperty *m_pPropSteps;
    QtVariantProperty *m_pPropOrientation;

    QSlider *m_pSlider;
};

#endif // AU_SLIDER_H

