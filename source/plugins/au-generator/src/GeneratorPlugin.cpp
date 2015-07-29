#include "GeneratorPlugin.h"
#include "Generator.h"

GeneratorPlugin::GeneratorPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon GeneratorPlugin::icon() const
{
    return QIcon(":/au-generator/generator.png");
}

AudioUnit* GeneratorPlugin::createInstance()
{
    return new Generator(this);
}
