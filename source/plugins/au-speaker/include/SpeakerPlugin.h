#include <QtPlugin>
#include "AudioUnitPlugin.h"

class SpeakerPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "SpeakerPlugin.json")

public:

    SpeakerPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
