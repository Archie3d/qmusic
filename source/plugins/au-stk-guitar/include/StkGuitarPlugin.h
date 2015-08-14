#include <QtPlugin>
#include "AudioUnitPlugin.h"

class StkGuitarPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "StkGuitarPlugin.json")

public:

    StkGuitarPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
