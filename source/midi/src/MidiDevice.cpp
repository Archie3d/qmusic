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
#include "RtMidi.h"
#include "mididevice.h"

MidiDevice::MidiDevice(Type type, int number)
    : m_valid(false),
      m_number(number),
      m_channel(1),
      m_type(type),
      m_name()
{
}

int MidiDevice::getNumberOfInputDevices()
{
    RtMidiIn midiIn;
    return midiIn.getPortCount();
}

int MidiDevice::getNumberOfOutputDevices()
{
    RtMidiOut midiOut;
    return midiOut.getPortCount();
}

QList<MidiDevice::Description> MidiDevice::enumerateDevices()
{
    QList<MidiDevice::Description> devs = enumerateInputDevices();
    devs.append(enumerateOutputDevices());
    return devs;
}

QList<MidiDevice::Description> MidiDevice::enumerateInputDevices()
{
    RtMidiIn midiIn;

    QList<MidiDevice::Description> devs;
    int n = midiIn.getPortCount();
    for (int i = 0; i < n; i++) {
        Description desc;
        desc.number = i;
        desc.type = Type_Input;
        try {
            desc.name = QString::fromStdString(midiIn.getPortName(i));
            devs.append(desc);
        } catch (RtMidiError &err) {
            Q_UNUSED(err);
        }
    }
    return devs;
}

QList<MidiDevice::Description> MidiDevice::enumerateOutputDevices()
{
    RtMidiOut midiOut;

    QList<MidiDevice::Description> devs;
    int n = midiOut.getPortCount();
    for (int i = 0; i < n; i++) {
        Description desc;
        desc.number = i;
        desc.type = Type_Output;
        try {
           desc.name = QString::fromStdString(midiOut.getPortName(i));
            devs.append(desc);
        } catch (RtMidiError &err) {
            Q_UNUSED(err);
        }
    }
    return devs;
    return devs;
}
