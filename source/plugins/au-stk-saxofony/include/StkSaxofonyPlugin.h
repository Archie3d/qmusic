#include <QtPlugin>
#include "AudioUnitPlugin.h"

class StkSaxofonyPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "StkSaxofonyPlugin.json")

public:

    StkSaxofonyPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
