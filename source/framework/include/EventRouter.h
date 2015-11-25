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

#ifndef EVENTROUTER_H
#define EVENTROUTER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include "IEventRouter.h"

/**
 * @brief Signal chain events router implementation.
 *
 * Event router accepts signal chain events and distributes
 * them to the rigistered listeners.
 *
 * There is normally a global event router residing in the
 * main \ref Application class.
 */
class EventRouter : public QObject,
                    public IEventRouter
{
    Q_OBJECT
public:

    /**
     * Construct event router.
     * @param pParent
     */
    EventRouter(QObject *pParent = nullptr);
    ~EventRouter();

    // IEventRouter interface
    void sendEvent(SignalChainEvent *pEvent);
    void postEvent(SignalChainEvent *pEvent);
    void processEvents();
    void registerHandler(IEventHandler *pHandler);
    void unregisterHandler(IEventHandler *pHandler);
    void purge();

signals:

    /// Internal signal used to trigger event handlers.
    void triggerProcessEvents();

private slots:

    /// Internal slot used to perform queued event processing.
    void doPprocessEvents();

private:

    /**
     * Clear an delete all unhandled events.
     */
    void purgeUnsafe();

    /**
     * Process a single event.
     * @param pEvent
     */
    void processEvent(SignalChainEvent *pEvent);

    /// List of registered handlers.
    QList<IEventHandler*> m_handlers;

    /// Pending events.
    QList<SignalChainEvent*> m_events;

    /// Protective mutex.
    mutable QMutex m_mutex;
};

#endif // EVENTROUTER_H

