#include "MixerPlugin.h"
#include "Mixer.h"

MixerPlugin::MixerPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon MixerPlugin::icon() const
{
    return QIcon(":/au-mixer/icon.png");
}

AudioUnit* MixerPlugin::createInstance()
{
    return new Mixer(this);
}
