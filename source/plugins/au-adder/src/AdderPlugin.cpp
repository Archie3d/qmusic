#include "AdderPlugin.h"
#include "Adder.h"

AdderPlugin::AdderPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon AdderPlugin::icon() const
{
    return QIcon(":/au-adder/icon.png");
}

AudioUnit* AdderPlugin::createInstance()
{
    return new Adder(this);
}
