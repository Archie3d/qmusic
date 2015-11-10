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

#ifndef PIANOKEYBOARDWINDOW_H
#define PIANOKEYBOARDWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"
#include "IEventRouter.h"

class PianoWidget;

class QMUSIC_VIEW_API PianoKeyboardWindow : public QDockWidget,
                                            public IEventHandler
{
    Q_OBJECT
public:

    PianoKeyboardWindow(QWidget *pParent = nullptr);

    // IEventHandler interface
    void handleEvent(SignalChainEvent *pEvent);

protected:

    void handleNoteOn(int number);
    void handleNoteOff(int number);

private:

    PianoWidget *m_pPianoWidget;
};

#endif // PIANOKEYBOARDWINDOW_H

