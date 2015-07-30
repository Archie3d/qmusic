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


MidiIn::MidiIn(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pMidiInputDevice = nullptr;

    m_outputFreqPtr = addOutput("f", QVariant::Double);
    m_outputVelocityPtr = addOutput("velocity", QVariant::Double);
    m_outputNoteOnPtr = addOutput("note on", QVariant::Bool);

    createProperties();
}

MidiIn::~MidiIn()
{
    delete m_pMidiInputDevice;
}

void MidiIn::processStart()
{
    int midiDevIndex = m_pPropMidiDevice->value().toInt();
    if (midiDevIndex < 0) {
        // Invalid index
        return;
    }
    QList<MidiDevice::Description> midiInputs = MidiDevice::enumerateInputDevices();

    if (midiInputs.count() <= midiDevIndex) {
        // List of input devices does not correspond
        return;
    }

    m_noteOn = false;
    m_frequency = 0.0;
    m_velocity = 0.0;

    m_pMidiInputDevice = new MidiInputDevice(midiInputs.at(midiDevIndex).number);
    m_pMidiInputDevice->addListener(this);
    if (!m_pMidiInputDevice->open()) {
        qCritical() << "Failed to open MIDI device";
    }
    m_pMidiInputDevice->start();
}

void MidiIn::processStop()
{
    if (m_pMidiInputDevice != nullptr) {
        m_pMidiInputDevice->stop();
        m_pMidiInputDevice->close();
        delete m_pMidiInputDevice;
        m_pMidiInputDevice = nullptr;
    }
}

void MidiIn::process()
{
    m_outputNoteOnPtr->setValue(m_noteOn);
    m_outputFreqPtr->setValue(m_frequency);
    m_outputVelocityPtr->setValue(m_velocity);
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
    QtVariantProperty *pRoot = rootProperty();

    // Enumerate MIDI devices
    QList<MidiDevice::Description> midiInputs = MidiDevice::enumerateInputDevices();

    m_pPropMidiDevice = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "MIDI Device");
    QVariantList list;
    foreach (const MidiDevice::Description &desc, midiInputs) {
        list.append(desc.name);
    }
    m_pPropMidiDevice->setAttribute("enumNames", list);
    m_pPropMidiDevice->setValue(0);

    // MIDI channel
    m_pPropChannel = propertyManager()->addProperty(QVariant::Int, "Channel");
    m_pPropChannel->setAttribute("minimum", 1);
    m_pPropChannel->setAttribute("maximum", 16);

    pRoot->addSubProperty(m_pPropMidiDevice);
    pRoot->addSubProperty(m_pPropChannel);
}
