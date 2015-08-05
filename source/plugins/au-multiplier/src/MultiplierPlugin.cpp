#include "MultiplierPlugin.h"
#include "Multiplier.h"

MultiplierPlugin::MultiplierPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon MultiplierPlugin::icon() const
{
    return QIcon(":/au-multiplier/icon.png");
}

AudioUnit* MultiplierPlugin::createInstance()
{
    return new Multiplier(this);
}
