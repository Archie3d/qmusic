#include "ConstantPlugin.h"
#include "Constant.h"

ConstantPlugin::ConstantPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon ConstantPlugin::icon() const
{
    return QIcon(":/au-constant/icon.png");
}

AudioUnit* ConstantPlugin::createInstance()
{
    return new Constant(this);
}
