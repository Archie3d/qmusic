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

#ifndef EXPOSEDINPUT_H
#define EXPOSEDINPUT_H

#include "FrameworkApi.h"
#include "AudioUnit.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;

/**
 * This is a special kind of audio unit that represents an exposed input port.
 */
class QMUSIC_FRAMEWORK_API ExposedInput : public AudioUnit
{
public:

    ExposedInput(AudioUnitPlugin *pPlugin);
    ~ExposedInput();

    QString exposedInputName() const;
    void setRefInputPort(InputPort *pInputPort);

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

    InputPort *m_pReferencedInputPort;

    QGraphicsSimpleTextItem *m_pNameItem;

    OutputPort *m_pOutput;
    QtVariantProperty *m_pPropName;
};

#endif // EXPOSEDINPUT_H

