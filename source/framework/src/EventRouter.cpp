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

#include "Application.h"
#include "SignalChainEvent.h"
#include "EventRouter.h"

EventRouter::EventRouter(QObject *pParent)
    : QObject(pParent),
      m_handlers(),
      m_events(),
      m_mutex()
{
    connect(this, SIGNAL(triggerProcessEvents()), this, SLOT(doPprocessEvents()), Qt::QueuedConnection);
}

EventRouter::~EventRouter()
{
    purgeUnsafe();
}

void EventRouter::sendEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    processEvent(pEvent);
    delete pEvent;
}

void EventRouter::postEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
    QMutexLocker lock(&m_mutex);
    m_events.append(pEvent);

    emit triggerProcessEvents();
}

void EventRouter::processEvents()
{
    emit triggerProcessEvents();
}

void EventRouter::registerHandler(IEventHandler *pHandler)
{
    Q_ASSERT(pHandler != nullptr);
    if (!m_handlers.contains(pHandler)) {
        m_handlers.append(pHandler);
    }
}

void EventRouter::unregisterHandler(IEventHandler *pHandler)
{
    m_handlers.removeOne(pHandler);
}

void EventRouter::purge()
{
    QMutexLocker lock(&m_mutex);
    purgeUnsafe();
}

void EventRouter::processEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
    for (IEventHandler *pHandler : m_handlers) {
        pHandler->handleEvent(pEvent);
    }
}

void EventRouter::doPprocessEvents()
{
    // Move pending events to a seeparate buffer for processing
    m_mutex.lock();
    QList<SignalChainEvent*> events = m_events;
    m_events.clear();
    m_mutex.unlock();

    // Process events
    for (SignalChainEvent *pEvent : events) {
        processEvent(pEvent);
        delete pEvent;
    }
}

void EventRouter::purgeUnsafe()
{
    qDeleteAll(m_events);
    m_events.clear();
}
