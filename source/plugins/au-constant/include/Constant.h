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

#ifndef AU_CONSTANT_H
#define AU_CONSTANT_H

#include "AudioUnit.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;

class Constant : public AudioUnit
{
public:

    Constant(AudioUnitPlugin *pPlugin);
    ~Constant();

protected:

    void processStart() override;
    void processStop() override;
    void process() override;
    void reset() override;

    QGraphicsItem* graphicsItem() override;
    QColor color() const override;
    int flags() const override;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

private:

    void createProperties();

    QGraphicsSimpleTextItem *m_pValueItem;

    OutputPort *m_pOutput;
    QtVariantProperty *m_pPropConstant;
};

#endif // AU_CONSTANT_H

