#include "AmplifierPlugin.h"
#include "Amplifier.h"

AmplifierPlugin::AmplifierPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon AmplifierPlugin::icon() const
{
    return QIcon(":/au-amplifier/icon.png");
}

AudioUnit* AmplifierPlugin::createInstance()
{
    return new Amplifier(this);
}
