#include "SpeakerPlugin.h"
#include "Speaker.h"

SpeakerPlugin::SpeakerPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon SpeakerPlugin::icon() const
{
    return QIcon(":/au-speaker/icon.png");
}

AudioUnit* SpeakerPlugin::createInstance()
{
    return new Speaker(this);
}
