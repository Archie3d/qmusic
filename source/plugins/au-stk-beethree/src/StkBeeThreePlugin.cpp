#include "StkBeeThreePlugin.h"
#include "StkBeeThree.h"

StkBeeThreePlugin::StkBeeThreePlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkBeeThreePlugin::icon() const
{
    return QIcon(":/au-stk-beethree/icon.png");
}

AudioUnit* StkBeeThreePlugin::createInstance()
{
    return new StkBeeThree(this);
}
