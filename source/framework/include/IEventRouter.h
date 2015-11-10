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

#ifndef IEVENTROUTER_H
#define IEVENTROUTER_H

class SignalChainEvent;

/**
 * @brief Interface to events handler.
 * This interface to be implemented by entities willing to
 * receive signal chain events.
 */
class IEventHandler
{
public:

    /**
     * @brief Process an event.
     * @note Defautl behavior should be doing nothing.
     * @param pEvent Pointer to an event.
     */
    virtual void handleEvent(SignalChainEvent *pEvent) = 0;

    /// Destructor.
    virtual ~IEventHandler() {}
};

/**
 * @brief Interface for signal chain events router.
 */
class IEventRouter
{
public:

    /**
     * Send an event to be processed immediately.
     * @param pEvent
     */
    virtual void sendEvent(SignalChainEvent *pEvent) = 0;

    /**
     * Post event to this signal chain.
     * @param evt Event to be posted.
     */
    virtual void postEvent(SignalChainEvent *pEvent) = 0;

    /**
     * Process all events waiting in the events queue.
     */
    virtual void processEvents() = 0;

    /**
     * Register event handler with with router.
     * @param pHandler
     */
    virtual void registerHandler(IEventHandler *pHandler) = 0;

    /**
     * Unregister event handler (remove from this router).
     * @param pHandler
     */
    virtual void unregisterHandler(IEventHandler *pHandler) = 0;

    /**
     * Remove all pending events.
     */
    virtual void purge() = 0;

    /// Destructor.
    virtual ~IEventRouter() {}
};

#endif // IEVENTROUTER_H

