#include "StkSaxofonyPlugin.h"
#include "StkSaxofony.h"

StkSaxofonyPlugin::StkSaxofonyPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkSaxofonyPlugin::icon() const
{
    return QIcon(":/au-stk-saxofony/icon.png");
}

AudioUnit* StkSaxofonyPlugin::createInstance()
{
    return new StkSaxofony(this);
}
