#include <QtPlugin>
#include "AudioUnitPlugin.h"

class StkClarinetPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "StkClarinetPlugin.json")

public:

    StkClarinetPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
