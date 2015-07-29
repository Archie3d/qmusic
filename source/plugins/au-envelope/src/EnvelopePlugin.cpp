#include "EnvelopePlugin.h"
#include "Envelope.h"

EnvelopePlugin::EnvelopePlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon EnvelopePlugin::icon() const
{
    return QIcon(":/au-envelope/icon.png");
}

AudioUnit* EnvelopePlugin::createInstance()
{
    return new Envelope(this);
}
