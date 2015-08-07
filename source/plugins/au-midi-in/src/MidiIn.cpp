#include <QDebug>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "MidiNote.h"
#include "MidiDevice.h"
#include "MidiInputDevice.h"
#include "ISignalChain.h"
#include "../include/MidiIn.h"

const QColor cDefaultColor(230, 240, 210);

MidiIn::MidiIn(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutputFreq = addOutput("f", QVariant::Double);
    m_pOutputVelocity = addOutput("velocity", QVariant::Double);
    m_pOutputNoteOn = addOutput("note on", QVariant::Bool);

    Application::instance()->midiInputDevice()->addListener(this);

    createProperties();
}

MidiIn::~MidiIn()
{
    Application::instance()->midiInputDevice()->removeListener(this);
}

QColor MidiIn::color() const
{
    return cDefaultColor;
}

void MidiIn::processStart()
{
    m_noteOn = false;
    m_frequency = 0.0;
    m_velocity = 0.0;
}

void MidiIn::processStop()
{
}

void MidiIn::process()
{
    m_pOutputNoteOn->setValue(m_noteOn);
    m_pOutputFreq->setValue(m_frequency);
    m_pOutputVelocity->setValue(m_velocity);
}

void MidiIn::reset()
{
}

void MidiIn::control(const QString &name, const QVariant &value)
{
}

void MidiIn::inputMidiMessage(const MidiMessage &msg)
{
    qDebug() << "[MIDI]" << msg;

    switch (msg.status()) {
    case MidiMessage::Status_NoteOn:
        m_noteOn = true;
        m_frequency = MidiNote(msg.noteNumber()).frequency();
        m_velocity = msg.velocity();
        break;
    case MidiMessage::Status_NoteOff:
        m_noteOn = false;
        m_frequency = MidiNote(msg.noteNumber()).frequency();
        m_velocity = msg.velocity();
        break;
    default:
        break;
    }
}

void MidiIn::createProperties()
{
}
