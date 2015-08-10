#include "GeneratorSinePlugin.h"
#include "GeneratorSine.h"

GeneratorSinePlugin::GeneratorSinePlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon GeneratorSinePlugin::icon() const
{
    return QIcon(":/au-generator-sine/icon.png");
}

AudioUnit* GeneratorSinePlugin::createInstance()
{
    return new GeneratorSine(this);
}
