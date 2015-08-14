#include "StkRhodeyPlugin.h"
#include "StkRhodey.h"

StkRhodeyPlugin::StkRhodeyPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkRhodeyPlugin::icon() const
{
    return QIcon(":/au-stk-rhodey/icon.png");
}

AudioUnit* StkRhodeyPlugin::createInstance()
{
    return new StkRhodey(this);
}
