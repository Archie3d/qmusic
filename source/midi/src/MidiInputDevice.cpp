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

#include <qglobal.h>
#include <QMap>
#include <QList>
#include "RtMidi.h"
#include "MidiMessage.h"
#include "MidiInputDevice.h"


// can be used for short, unsigned short, word, unsigned word (2-byte types)
#define BYTESWAP16(n) (((n&0xFF00)>>8)|((n&0x00FF)<<8))

// can be used for int or unsigned int or float (4-byte types)
#define BYTESWAP32(n) ((BYTESWAP16((n&0xFFFF0000)>>16))|((BYTESWAP16(n&0x0000FFFF))<<16))

// can be used for unsigned long long or double (8-byte types)
#define BYTESWAP64(n) ((BYTESWAP32((n&0xFFFFFFFF00000000)>>32))|((BYTESWAP32(n&0x00000000FFFFFFFF))<<32))

struct MidiInputDevicePrivate
{
    RtMidiIn *pMidiIn;
    bool deviceIsOpen;
    QList<IMidiInputListener*> listeners;
};

/// MIDI input callback function
void midiInCallback(double deltatime, std::vector<unsigned char> *pMessage, void *pUserData )
{
    Q_ASSERT(pUserData != nullptr);
    MidiInputDevice *pDev = static_cast<MidiInputDevice*>(pUserData);

    size_t nBytes = pMessage->size();
    for (unsigned int i = 0; i < nBytes; i++) {
        qDebug() << i << (int)pMessage->at(i);
    }

    // TODO pass word value here
    pDev->acceptMessage(0);
}

MidiInputDevice::MidiInputDevice(int number)
    : MidiDevice(Type_Input, number)
{
    m = new MidiInputDevicePrivate;
    m->pMidiIn = new RtMidiIn();
    m->deviceIsOpen = false;
    setValid(validateDevice());
}

MidiInputDevice::~MidiInputDevice()
{
    close();
    delete m->pMidiIn;
    delete m;
}

bool MidiInputDevice::open()
{
    Q_ASSERT(m->pMidiIn != nullptr);

    if (isOpen()) {
        // Device is already open.
        qWarning() << "MIDI In device is already open";
        return true;
    }

    m->pMidiIn->openPort(number());
    // Do not ignore  sysex, timing, or active sensing
    m->pMidiIn->ignoreTypes(false, false, false);

    m->deviceIsOpen = true;
    return true;
}

bool MidiInputDevice::isOpen() const
{
    return m->deviceIsOpen;
}

bool MidiInputDevice::start()
{
    if (!isOpen()) {
        qCritical() << "MIDI In device is open open, unable to start";
        return false;
    }

    m->pMidiIn->setCallback(&midiInCallback, this);
    return true;
}

bool MidiInputDevice::stop()
{
    if (!isOpen()) {
        qCritical() << "MIDI In device is not open, unable to stop";
        return false;
    }

    m->pMidiIn->cancelCallback();
    return true;
}


bool MidiInputDevice::close()
{
    if (!isOpen()) {
        return true;
    }

    m->pMidiIn->closePort();

    m->deviceIsOpen = false;
    return true;
}

void MidiInputDevice::addListener(IMidiInputListener *pListener)
{
    if (pListener != nullptr) {
        m->listeners.append(pListener);
    }
}

void MidiInputDevice::removeListener(IMidiInputListener *pListener)
{
    m->listeners.removeAll(pListener);
}

void MidiInputDevice::acceptMessage(unsigned int msg)
{
    if (!m->deviceIsOpen) {
        return;
    }

    // Message received.
    MidiMessage midiMessage(msg);

    // Pass the message in case of the channel match.
    if (midiMessage.channel() == channel()) {
        notifyListeners(midiMessage);
    }
}

bool MidiInputDevice::validateDevice()
{
    if (number() < 0 || number() >= getNumberOfInputDevices()) {
        // Device number is out of range.
        return false;
    }

    QString devName = QString::fromStdString(m->pMidiIn->getPortName(number()));
    setName(devName);
    return true;
}

void MidiInputDevice::notifyListeners(const MidiMessage &msg)
{
    foreach (IMidiInputListener *pListener, m->listeners) {
        pListener->inputMidiMessage(msg);
    }
}
