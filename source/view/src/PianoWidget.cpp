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
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QBrush>
#include <QFont>
#include "MidiNote.h"
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

PianoWidget::PianoWidget(QWidget *pParent)
    : QWidget(pParent),
      m_onNotes()
{
    reset();
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

void PianoWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double keyWidth = double(width()) / double(numberOfWhiteKeys());
    double blackKeyWidth = keyWidth * 2 / 3;

    QPen pen;
    pen.setColor(QColor("gray"));
    QBrush whiteBrush(QColor("white"));
    QBrush blackBrush(QColor("black"));
    QBrush onBrush(QColor(0xCC, 0xDD, 0xFF));

    painter.setPen(pen);

    QFont font("Arial");
    font.setPixelSize(keyWidth / 3 * 2);
    painter.setFont(font);

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
        painter.drawRect(r);

        // Draw black keys
        if (note.hasFlat() &&  i > 0) { // Skip C-1b
            MidiNote flatNote = note.flat();
            if (isNoteOn(flatNote.number())) {
                painter.setBrush(onBrush);
            } else {
                painter.setBrush(blackBrush);
            }

            QRectF r(pos - blackKeyWidth / 2, 0, blackKeyWidth, height() * 2 / 3);
            painter.drawRect(r);
        }

        // Draw C keys labels.
        if (note.note() == MidiNote::Note_C) {
            painter.drawText(pos + 1, height() - 2, note.toString());
        }
    }
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
