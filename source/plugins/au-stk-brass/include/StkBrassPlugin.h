#include <QtPlugin>
#include "AudioUnitPlugin.h"

class StkBrassPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "StkBrassPlugin.json")

public:

    StkBrassPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
