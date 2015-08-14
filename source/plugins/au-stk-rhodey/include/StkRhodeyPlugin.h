#include <QtPlugin>
#include "AudioUnitPlugin.h"

class StkRhodeyPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "StkRhodeyPlugin.json")

public:

    StkRhodeyPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
