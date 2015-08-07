#include "DelayPlugin.h"
#include "Delay.h"

DelayPlugin::DelayPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon DelayPlugin::icon() const
{
    return QIcon(":/au-delay/icon.png");
}

AudioUnit* DelayPlugin::createInstance()
{
    return new Delay(this);
}
