#include "MidiInPlugin.h"
#include "MidiIn.h"

MidiInPlugin::MidiInPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon MidiInPlugin::icon() const
{
    return QIcon(":/au-midi-in/icon.png");
}

AudioUnit* MidiInPlugin::createInstance()
{
    return new MidiIn(this);
}
