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

#ifndef IAUDIOUNIT_H
#define IAUDIOUNIT_H

#include <QString>
#include <QVariant>
#include <QGraphicsItem>
#include "ISerializable.h"
#include "FrameworkApi.h"

/**
 * @brief Audio unit interface.
 *
 * This is an interface to an audio unit. An audio unit implements
 * a signle step of signal ptocessing. It takes whatever is currently
 * provided on its inputs and generates outputs.
 */
class QMUSIC_FRAMEWORK_API IAudioUnit : public ISerializable
{
public:

    /**
     * Flags affecting the audio unit behavior and representation.
     */
    enum Flag {
        Flag_NoFlags = 0,
        Flag_NoTitle = 1,   ///< No title should be drawn.
        Flag_NoFrame = 2,   ///< No frame should ne drawn.
        Flag_SingleInstance = 4 ///< Only one instance of the unit is allowed.
    };


    // Destructor.
    virtual ~IAudioUnit() {}

    /**
     * Prepare for the update.
     */
    virtual void prepareUpdate() = 0;

    /**
     * Perform processing on the current state.
     */
    virtual void update() = 0;

    /**
     * Start processing.
     */
    virtual void start() = 0;

    /**
     * Stop processing.
     */
    virtual void stop() = 0;

    /**
     * @brief Tells whether the unit has been started.
     * @return
     */
    virtual bool isStarted() const = 0;

    /**
     * @brief Reset the audio unit to its initial state.
     */
    virtual void reset() = 0;

    /**
     * @brief Create a custom graphics view for this item
     * @return
     */
    virtual QGraphicsItem* graphicsItem() = 0;

    /**
     * @brief Audio unit representation color.
     * @return
     */
    virtual QColor color() const = 0;

    /**
     * Returns audio unit flags.
     * @return
     */
    virtual int flags() const = 0;
};

#endif // IAUDIOUNIT_H

