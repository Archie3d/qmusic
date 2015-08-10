#include <QtPlugin>
#include "AudioUnitPlugin.h"

class GeneratorSinePlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "GeneratorSinePlugin.json")

public:

    GeneratorSinePlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    AudioUnit* createInstance();
};
