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

#include <QVector>
#include <QMap>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QBrush>
#include <QFont>
#include "Application.h"
#include "EventRouter.h"
#include "MidiNote.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "PianoWidget.h"

const QVector<MidiNote::Note> cWhiteNotes = {
    MidiNote::Note_C,
    MidiNote::Note_D,
    MidiNote::Note_E,
    MidiNote::Note_F,
    MidiNote::Note_G,
    MidiNote::Note_A,
    MidiNote::Note_B
};

const QMap<Qt::Key, MidiNote> cKeysToMotesMap = {
    { Qt::Key_Z, MidiNote(MidiNote::Note_C, 3)  },
    { Qt::Key_S, MidiNote(MidiNote::Note_Db, 3) },
    { Qt::Key_X, MidiNote(MidiNote::Note_D, 3)  },
    { Qt::Key_D, MidiNote(MidiNote::Note_Eb, 3) },
    { Qt::Key_C, MidiNote(MidiNote::Note_E, 3)  },
    { Qt::Key_V, MidiNote(MidiNote::Note_F, 3)  },
    { Qt::Key_G, MidiNote(MidiNote::Note_Gb, 3) },
    { Qt::Key_B, MidiNote(MidiNote::Note_G, 3)  },
    { Qt::Key_H, MidiNote(MidiNote::Note_Ab, 3) },
    { Qt::Key_N, MidiNote(MidiNote::Note_A, 3)  },
    { Qt::Key_J, MidiNote(MidiNote::Note_Bb, 3) },
    { Qt::Key_M, MidiNote(MidiNote::Note_B, 3)  },

    { Qt::Key_Comma, MidiNote(MidiNote::Note_C, 4)      },
    { Qt::Key_L, MidiNote(MidiNote::Note_Db, 4)         },
    { Qt::Key_Period, MidiNote(MidiNote::Note_D, 4)     },
    { Qt::Key_Semicolon, MidiNote(MidiNote::Note_Eb, 4) },
    { Qt::Key_Slash, MidiNote(MidiNote::Note_E, 4)      },

    { Qt::Key_Q, MidiNote(MidiNote::Note_C, 4)  },
    { Qt::Key_2, MidiNote(MidiNote::Note_Db, 4) },
    { Qt::Key_W, MidiNote(MidiNote::Note_D, 4)  },
    { Qt::Key_3, MidiNote(MidiNote::Note_Eb, 4) },
    { Qt::Key_E, MidiNote(MidiNote::Note_E, 4)  },
    { Qt::Key_R, MidiNote(MidiNote::Note_F, 4)  },
    { Qt::Key_5, MidiNote(MidiNote::Note_Gb, 4) },
    { Qt::Key_T, MidiNote(MidiNote::Note_G, 4)  },
    { Qt::Key_6, MidiNote(MidiNote::Note_Ab, 4) },
    { Qt::Key_Y, MidiNote(MidiNote::Note_A, 4)  },
    { Qt::Key_7, MidiNote(MidiNote::Note_Bb, 4) },
    { Qt::Key_U, MidiNote(MidiNote::Note_B, 4)  },

    { Qt::Key_I, MidiNote(MidiNote::Note_C, 5)  },
    { Qt::Key_9, MidiNote(MidiNote::Note_Db, 5) },
    { Qt::Key_O, MidiNote(MidiNote::Note_D, 5)  },
    { Qt::Key_0, MidiNote(MidiNote::Note_Eb, 5) },
    { Qt::Key_P, MidiNote(MidiNote::Note_E, 5)  },

    { Qt::Key_BracketLeft, MidiNote(MidiNote::Note_F, 5)  },
    { Qt::Key_Equal, MidiNote(MidiNote::Note_Gb, 5)       },
    { Qt::Key_BracketRight, MidiNote(MidiNote::Note_G, 5) }
};

const double cBlackKeyWidth = 0.6;
const double cBlackKeyHeight = 2.0 / 3.0;

PianoWidget::PianoWidget(QWidget *pParent)
    : QWidget(pParent),
      m_onNotes()
{
    calculateKeysRects();
    reset();

    setFocusPolicy(Qt::StrongFocus);
}

void PianoWidget::reset()
{
    for (int i = 0; i < 128; i++) {
        m_onNotes[i] = false;
    }
    repaint();
}

void PianoWidget::setNote(int number, bool on)
{
    if (number >= 0 && number < 128) {
        m_onNotes[number] = on;
    }
    repaint();
}

void PianoWidget::resizeEvent(QResizeEvent *pEvent)
{
    calculateKeysRects();
    QWidget::resizeEvent(pEvent);
}

void PianoWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setColor(QColor("gray"));
    QBrush whiteBrush(QColor(255, 250, 240));
    QBrush blackBrush(QColor("black"));
    QBrush onBrush(QColor(0xCC, 0xDD, 0xFF));

    painter.setPen(pen);

    QFont font("Arial");
    font.setPixelSize(whiteKeyWidth() * cBlackKeyWidth);
    painter.setFont(font);

    // Paint white keys first
    for (int n : m_keysRects.keys()) {
        MidiNote note(n);
        if (note.isNatural()) {
            QRectF rect = m_keysRects[n];
            painter.setBrush(isNoteOn(n) ? onBrush : whiteBrush);
            drawKey(painter, rect);

            // Draw C key labels
            if (note.note() == MidiNote::Note_C) {
                painter.drawText(rect.left() + 1, height() - 2, note.toString());
            }

        }
    }

    // Paint black keys on top
    for (int n : m_keysRects.keys()) {
        MidiNote note(n);
        if (!note.isNatural()) {
            QRectF rect = m_keysRects[n];
            painter.setBrush(isNoteOn(n) ? onBrush : blackBrush);
            drawKey(painter, rect);
        }
    }

#if  0
    // Draw white keys
    for (int i = 0; i < numberOfWhiteKeys(); i++) {
        int octave = (i / 7) - 1;
        int n = i % 7;
        MidiNote note(cWhiteNotes.at(n), octave);

        int pos = keyWidth * double(i);
        QRectF r(pos, 0, keyWidth, height());
        if (isNoteOn(note.number())) {
            painter.setBrush(onBrush);
        } else {
            painter.setBrush(whiteBrush);
        }

        drawKey(painter, r);

        // Draw black keys
        if (note.hasFlat() &&  i > 0) { // Skip C-1b
            MidiNote flatNote = note.flat();
            if (isNoteOn(flatNote.number())) {
                painter.setBrush(onBrush);
            } else {
                painter.setBrush(blackBrush);
            }

            // Move Db, Gb, and Eb, Bb slightly apart.
            float bpos = 0.0f;
            if (flatNote.note() == MidiNote::Note_Db || flatNote.note() == MidiNote::Note_Gb) {
                bpos = -1.0f;
            } else if (flatNote.note() == MidiNote::Note_Eb || flatNote.note() == MidiNote::Note_Bb) {
                bpos = 1.0f;
            }
            bpos = pos + blackKeyWidth * 0.15f * bpos;

            QRectF r(bpos - blackKeyWidth / 2, 0, blackKeyWidth, blackKeyHeight());
            drawKey(painter, r);
        }

        // Draw C keys labels.
        if (note.note() == MidiNote::Note_C) {
            painter.drawText(pos + 1, height() - 2, note.toString());
        }
    }

#endif

    // Draw a shadow on top
    QLinearGradient shadow(QPointF(0.0f, 0.0f), QPointF(0.0f, 16.0f));
    shadow.setColorAt(0.0, QColor(0, 0, 0, 128));
    shadow.setColorAt(1.0, QColor(0, 0, 0, 0));

    painter.fillRect(0, 0, width()-1, 16, shadow);
}

void PianoWidget::drawKey(QPainter &painter, const QRectF &rect) const
{
    QRectF r(rect);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(r, 3, 3);
    r.setHeight(r.height() - 3);
    path.addRect(r);
    path = path.simplified();

    painter.drawPath(path);
}

void PianoWidget::keyPressEvent(QKeyEvent *pEvent)
{
    Qt::Key key = static_cast<Qt::Key>(pEvent->key());
    if (!pEvent->isAutoRepeat() && cKeysToMotesMap.contains(key)) {
        MidiNote note = cKeysToMotesMap.value(key);

        sendNoteOn(note.number(), 100);
    }

    QWidget::keyPressEvent(pEvent);
}

void PianoWidget::keyReleaseEvent(QKeyEvent *pEvent)
{
    Qt::Key key = static_cast<Qt::Key>(pEvent->key());
    if (!pEvent->isAutoRepeat() && cKeysToMotesMap.contains(key)) {
        MidiNote note = cKeysToMotesMap.value(key);

        sendNoteOff(note.number(), 100);
    }

    QWidget::keyReleaseEvent(pEvent);
}

void PianoWidget::mousePressEvent(QMouseEvent *pEvent)
{
    int n = mousePosToNoteNumber(pEvent->pos());
    if (n >= 0) {
        int velocity = velocityForMousePos(pEvent->pos(), n);
        sendNoteOn(n, velocity);
        m_mouseNoteNumber = n;
    }
    QWidget::mousePressEvent(pEvent);
}

void PianoWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    int n = mousePosToNoteNumber(pEvent->pos());
    if (n >= 0) {
        int velocity = velocityForMousePos(pEvent->pos(), n);
        sendNoteOff(n, velocity);
        if (m_mouseNoteNumber != n) {
            // Also release stuck key
            sendNoteOff(m_mouseNoteNumber, velocity);
        }
        m_mouseNoteNumber = -1;
    }

    QWidget::mouseReleaseEvent(pEvent);
}

void PianoWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    int n = mousePosToNoteNumber(pEvent->pos());
    if (n != m_mouseNoteNumber) {
        int velocity = velocityForMousePos(pEvent->pos(), n);
        sendNoteOn(n, velocity);
        sendNoteOff(m_mouseNoteNumber, velocity);
        m_mouseNoteNumber = n;
    }
    QWidget::mouseMoveEvent(pEvent);
}

bool PianoWidget::eventFilter(QObject *pObject, QEvent *pEvent)
{
    Q_UNUSED(pObject);

    if (pEvent->type() == QEvent::KeyPress) {
        QKeyEvent *pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent);
        keyPressEvent(pKeyEvent);
    } else if (pEvent->type() == QEvent::KeyRelease) {
        QKeyEvent *pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent);
        keyReleaseEvent(pKeyEvent);
    }
    return false;
}

int PianoWidget::numberOfOctaves() const
{
    return 10;
}

int PianoWidget::numberOfWhiteKeys() const
{
    return numberOfOctaves() * 7;
}

bool PianoWidget::isNoteOn(int number) const
{
    return m_onNotes.value(number, false);
}

int PianoWidget::mousePosToNoteNumber(const QPoint &pos) const
{
    // Check the black keys first
    for (int n : m_keysRects.keys()) {
        MidiNote note(n);
        if (!note.isNatural()) {
            QRectF rect = m_keysRects[n];
            if (rect.contains(pos)) {
                return n;
            }
        }
    }

    // Check the white keys then
    for (int n : m_keysRects.keys()) {
        MidiNote note(n);
        if (note.isNatural()) {
            QRectF rect = m_keysRects[n];
            if (rect.contains(pos)) {
                return n;
            }
        }
    }

    return -1;
}

double PianoWidget::whiteKeyWidth() const
{
    return double(width()) / double(numberOfWhiteKeys());
}

void PianoWidget::calculateKeysRects()
{
    double keyWidth = whiteKeyWidth();
    double blackKeyWidth = keyWidth * cBlackKeyWidth;

    // White keys
    for (int i = 0; i < numberOfWhiteKeys(); i++) {
        int octave = (i / 7) - 1;
        int n = i % 7;
        MidiNote note(cWhiteNotes.at(n), octave);

        int pos = keyWidth * double(i);
        QRectF r(pos, 0, keyWidth, height());

        m_keysRects[note.number()] = r;

        // Black keys
        if (note.hasFlat() &&  i > 0) { // Skip C-1b
            MidiNote flatNote = note.flat();

            // Move Db, Gb, and Eb, Bb slightly apart.
            float bpos = 0.0f;
            if (flatNote.note() == MidiNote::Note_Db || flatNote.note() == MidiNote::Note_Gb) {
                bpos = -1.0f;
            } else if (flatNote.note() == MidiNote::Note_Eb || flatNote.note() == MidiNote::Note_Bb) {
                bpos = 1.0f;
            }
            bpos = pos + blackKeyWidth * 0.15f * bpos;

            QRectF r(bpos - blackKeyWidth / 2, 0, blackKeyWidth, height() * cBlackKeyHeight);

            m_keysRects[flatNote.number()] = r;
        }

    }
}

int PianoWidget::velocityForMousePos(const QPoint &pos, int noteNumber) const
{
    return pos.y() * 127 / m_keysRects.value(noteNumber).height();
}

void PianoWidget::sendNoteOn(int number, int velocity)
{
    if (!m_onNotes[number]) {
        NoteOnEvent *pNoteOnEvent = new NoteOnEvent(number, velocity);
        Application::instance()->eventRouter()->postEvent(pNoteOnEvent);
    }
}

void PianoWidget::sendNoteOff(int number, int velocity)
{
    if (m_onNotes[number]) {
        NoteOffEvent *pNoteOffEvent = new NoteOffEvent(number, velocity);
        Application::instance()->eventRouter()->postEvent(pNoteOffEvent);
    }
}
