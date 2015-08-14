#include <QtPlugin>
#include "AudioUnitPlugin.h"

class StkBeeThreePlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "StkBeeThreePlugin.json")

public:

    StkBeeThreePlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
