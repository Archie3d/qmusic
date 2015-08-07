#include <QtPlugin>
#include "AudioUnitPlugin.h"

class MidiInCtrlPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "MidiInCtrlPlugin.json")

public:

    MidiInCtrlPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
