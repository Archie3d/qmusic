#include "StkFlutePlugin.h"
#include "StkFlute.h"

StkFlutePlugin::StkFlutePlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkFlutePlugin::icon() const
{
    return QIcon(":/au-stk-flute/icon.png");
}

AudioUnit* StkFlutePlugin::createInstance()
{
    return new StkFlute(this);
}
