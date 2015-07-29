#include "../include/InputPlugin.h"
#include "../include/Input.h"

InputPlugin::InputPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon InputPlugin::icon() const
{
    return QIcon(":/au-input/icon.png");
}

AudioUnit* InputPlugin::createInstance()
{
    return new Input(this);
}
