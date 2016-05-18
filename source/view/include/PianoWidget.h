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

    void resizeEvent(QResizeEvent *pEvent);

    void paintEvent(QPaintEvent *pEvent);

    void keyPressEvent(QKeyEvent *pEvent);
    void keyReleaseEvent(QKeyEvent *pEvent);

    void mousePressEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);

    /**
     * @brief Event filter used to capture keyboard events.
     * @param pObject Sender.
     * @param pEvent Event.
     * @return true if event has been processed.
     */
    bool eventFilter(QObject *pObject, QEvent *pEvent);

private:

    int numberOfOctaves() const;
    int numberOfKeys() const;
    int numberOfWhiteKeys() const;

    /**
     * @brief Draw a single piano key.
     * @param painter
     * @param rect
     */
    void drawKey(QPainter &painter, const QRectF &rect) const;

    /**
     * Tells whether the note is on.
     * @param number NoteNumber
     * @return true if note is on.
     */
    bool isNoteOn(int number) const;

    /**
     * @brief Returns note number that corresponds to the mouse cursor position on the piano keyboard.
     * @param pos Mouse cursor position.
     * @return Corresponding note number.
     */
    int mousePosToNoteNumber(const QPoint &pos) const;

    /**
     * @brief Returns white key width depending on the widget current width.
     * @return White key width (in pixels).
     */
    double whiteKeyWidth() const;

    /**
     * @brief Claculate drawing rectangles for all the keys.
     * This is update the map of keys painting rectangles used
     * to paint this widget and track the mouse clicks to the keys.
     */
    void calculateKeysRects();

    /**
     * @brief Compute the note velocity depending on the mouse click position.
     * @param pos Mouse click posititon.
     * @param noteNumber Note number being clicked.
     * @return MIDI note velocity (0..127).
     */
    int velocityForMousePos(const QPoint &pos, int noteNumber) const;

    /**
     * @brief Send note-on event.
     * @param number MIDI note number.
     * @param velocity Note velocity (0..127).
     */
    void sendNoteOn(int number, int velocity);

    /**
     * @brief Send note-off event.
     * @param number MIDI note number.
     * @param velocity Mote velocity (0..127).
     */
    void sendNoteOff(int number, int velocity);

    /// Map note number to its on/off state
    QMap<int, bool> m_onNotes;

    /// Map note to the key rectange (as it is painted).
    QMap<int, QRectF> m_keysRects;

    /// Current note number triggered by the mouse click.
    int m_mouseNoteNumber;
};

#endif // PIANOWIDGET_H

