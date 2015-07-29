#include <QtPlugin>
#include "AudioUnitPlugin.h"

class AudioSinkPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "AudioSinkPlugin.json")

public:

    AudioSinkPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
