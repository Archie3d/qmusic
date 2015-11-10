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

#include <QHBoxLayout>
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "PianoWidget.h"
#include "PianoKeyboardWindow.h"

PianoKeyboardWindow::PianoKeyboardWindow(QWidget *pParent)
    : QDockWidget(pParent)
{
    setObjectName("pianoKeyboardWindow");
    setWindowTitle(tr("Piano keyboard"));

    m_pPianoWidget = new PianoWidget();
    setWidget(m_pPianoWidget);
}

void PianoKeyboardWindow::handleEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
    switch (pEvent->type()) {
    case SignalChainEvent::NoteOn: {
        NoteOnEvent *pNoteOnEvent = dynamic_cast<NoteOnEvent*>(pEvent);
        Q_ASSERT(pNoteOnEvent != nullptr);
        handleNoteOn(pNoteOnEvent->noteNumber());
        break;
    }
    case SignalChainEvent::NoteOff: {
        NoteOffEvent *pNoteOffEvent = dynamic_cast<NoteOffEvent*>(pEvent);
        Q_ASSERT(pNoteOffEvent != nullptr);
        handleNoteOff(pNoteOffEvent->noteNumber());
        break;
    }
    default:
        break;
    }
}

void PianoKeyboardWindow::handleNoteOn(int number)
{
    m_pPianoWidget->setNote(number, true);
}

void PianoKeyboardWindow::handleNoteOff(int number)
{
    m_pPianoWidget->setNote(number, false);
}
