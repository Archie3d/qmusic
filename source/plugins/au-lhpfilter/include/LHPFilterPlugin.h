#include <QtPlugin>
#include "AudioUnitPlugin.h"

class LHPFilterPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "LHPFilterPlugin.json")

public:

    LHPFilterPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
