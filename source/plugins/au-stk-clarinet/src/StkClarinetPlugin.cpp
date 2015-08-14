#include "StkClarinetPlugin.h"
#include "StkClarinet.h"

StkClarinetPlugin::StkClarinetPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon StkClarinetPlugin::icon() const
{
    return QIcon(":/au-stk-clarinet/icon.png");
}

AudioUnit* StkClarinetPlugin::createInstance()
{
    return new StkClarinet(this);
}
