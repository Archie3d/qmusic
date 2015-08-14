#include "StkBrassPlugin.h"
#include "StkBrass.h"

StkBrassPlugin::StkBrassPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkBrassPlugin::icon() const
{
    return QIcon(":/au-stk-brass/icon.png");
}

AudioUnit* StkBrassPlugin::createInstance()
{
    return new StkBrass(this);
}
