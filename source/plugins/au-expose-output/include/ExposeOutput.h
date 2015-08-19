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

#ifndef AU_EXPOSE_OUTPUT_H
#define AU_EXPOSE_OUTPUT_H

#include "AudioUnit.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;

class ExposeOutput : public AudioUnit
{
public:

    ExposeOutput(AudioUnitPlugin *pPlugin);
    ~ExposeOutput();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    QGraphicsItem* graphicsItem();
    QColor color() const override;
    int flags() const;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

private:

    void createProperties();

    QGraphicsSimpleTextItem *m_pNameItem;

    InputPort *m_pInput;
    QtVariantProperty *m_pPropName;
};

#endif // AU_EXPOSE_OUTPUT_H

