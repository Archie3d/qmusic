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

#ifndef PIANOWIDGET_H
#define PIANOWIDGET_H

#include <QMap>
#include <QWidget>
#include "ViewApi.h"

/**
 * Draw piano keyboard containing alll MIDI notes.
 */
class QMUSIC_VIEW_API PianoWidget : public QWidget
{
    Q_OBJECT

public:

    PianoWidget(QWidget *pParent = nullptr);

    void reset();

    /**
     * Set note on or off.
     * @param number
     * @param on
     */
    void setNote(int number, bool on);

protected:

    void paintEvent(QPaintEvent *pEvent);

    void keyPressEvent(QKeyEvent *pEvent);
    void keyReleaseEvent(QKeyEvent *pEvent);

    bool eventFilter(QObject *pObject, QEvent *pEvent);

private:

    int numberOfOctaves() const;
    int numberOfKeys() const;
    int numberOfWhiteKeys() const;

    /**
     * Tells whether the note is on.
     * @param number NoteNumber
     * @return
     */
    bool isNoteOn(int number) const;

    ///< Map note number to its on/off state
    QMap<int, bool> m_onNotes;
};

#endif // PIANOWIDGET_H

