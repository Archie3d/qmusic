#include <QtPlugin>
#include "AudioUnitPlugin.h"

class DelayPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "DelayPlugin.json")

public:

    DelayPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
