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

#ifndef EXPOSEDOUTPUT_H
#define EXPOSEDOUTPUT_H

#include "FrameworkApi.h"
#include "AudioUnit.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;

/**
 * @brief A special kind of audio unit that represents an exposed output.
 *
 * Exposed output unit is used in sub-chains. It acts as areference point for
 * the sub-chains containers, making the output being exposed outside of the container.
 *
 * @see ExposedInput
 */
class QMUSIC_FRAMEWORK_API ExposedOutput : public AudioUnit
{
public:

    ExposedOutput(AudioUnitPlugin *pPlugin);
    ~ExposedOutput();

    QString exposedOutputName() const;
    void setRefOutputPort(OutputPort *pOutputPort);

    void fastUpdate();

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

    OutputPort *m_pReferencedOutputPort;

    QGraphicsSimpleTextItem *m_pNameItem;

    InputPort *m_pInput;
    QtVariantProperty *m_pPropName;

    QList<AudioUnit*> m_updateChain;
};

#endif // EXPOSEDOUTPUT_H

