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
#ifdef Q_OS_WIN
#   include <Windows.h>
#   include <mmsystem.h>
#endif
#include "mididevice.h"

MidiDevice::MidiDevice(Type type, int number)
    : m_valid(false),
      m_number(number),
      m_channel(1),
      m_type(type),
      m_name(),
      m_manufacturerId(0),
      m_productId(0)
{
}

int MidiDevice::getNumberOfInputDevices()
{
    return midiInGetNumDevs();
}

int MidiDevice::getNumberOfOutputDevices()
{
    return midiOutGetNumDevs();
}

QList<MidiDevice::Description> MidiDevice::enumerateDevices()
{
    QList<MidiDevice::Description> devs = enumerateInputDevices();
    devs.append(enumerateOutputDevices());
    return devs;
}

QList<MidiDevice::Description> MidiDevice::enumerateInputDevices()
{
    QList<MidiDevice::Description> devs;
    int n = getNumberOfInputDevices();
    for (int i = 0; i < n; i++) {
        MIDIINCAPS devCaps;
        MMRESULT r = midiInGetDevCaps(i,
                                      &devCaps,
                                      sizeof(MIDIINCAPS));

        if (r == MMSYSERR_NOERROR) {
            Description desc;
            desc.number = i;
            desc.type = Type_Input;
            //desc.name = QString::fromWCharArray(devCaps.szPname);
            desc.name = QString::fromLatin1(devCaps.szPname);
            devs.append(desc);
        }
    }
    return devs;
}

QList<MidiDevice::Description> MidiDevice::enumerateOutputDevices()
{
    QList<MidiDevice::Description> devs;
    int n = getNumberOfOutputDevices();
    for (int i = 0; i < n; i++) {
        MIDIOUTCAPS devCaps;
        MMRESULT r = midiOutGetDevCaps(i,
                                      &devCaps,
                                      sizeof(MIDIOUTCAPS));

        if (r == MMSYSERR_NOERROR) {
            Description desc;
            desc.number = i;
            desc.type = Type_Output;
            //desc.name = QString::fromWCharArray(devCaps.szPname);
            desc.name = QString::fromLatin1(devCaps.szPname);
            devs.append(desc);
        }
    }
    return devs;
    return devs;
}
