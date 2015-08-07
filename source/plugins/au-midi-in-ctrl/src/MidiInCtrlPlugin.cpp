#include "MidiInCtrlPlugin.h"
#include "MidiInCtrl.h"

MidiInCtrlPlugin::MidiInCtrlPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon MidiInCtrlPlugin::icon() const
{
    return QIcon(":/au-midi-in-ctrl/icon.png");
}

AudioUnit* MidiInCtrlPlugin::createInstance()
{
    return new MidiInCtrl(this);
}
