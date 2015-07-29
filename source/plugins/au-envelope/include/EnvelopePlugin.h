#include <QtPlugin>
#include "AudioUnitPlugin.h"

class EnvelopePlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "EnvelopePlugin.json")

public:

    EnvelopePlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
