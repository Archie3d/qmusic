#include "StkGuitarPlugin.h"
#include "StkGuitar.h"

StkGuitarPlugin::StkGuitarPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkGuitarPlugin::icon() const
{
    return QIcon(":/au-stk-guitar/icon.png");
}

AudioUnit* StkGuitarPlugin::createInstance()
{
    return new StkGuitar(this);
}
