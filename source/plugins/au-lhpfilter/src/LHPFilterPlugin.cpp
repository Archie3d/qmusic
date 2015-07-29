#include "LHPFilterPlugin.h"
#include "LHPFilter.h"

LHPFilterPlugin::LHPFilterPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon LHPFilterPlugin::icon() const
{
    return QIcon(":/au-lhpfilter/icon.png");
}

AudioUnit* LHPFilterPlugin::createInstance()
{
    return new LHPFilter(this);
}
