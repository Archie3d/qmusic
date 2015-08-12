#include <QtPlugin>
#include <QMap>
#include "AudioUnitPlugin.h"

class OpenAirPlugin : public AudioUnitPlugin
{
    Q_OBJECT
    Q_INTERFACES(AudioUnitPlugin)
    Q_PLUGIN_METADATA(IID "qmusic.audiounits.plugin" FILE "OpenAirPlugin.json")

public:

    OpenAirPlugin(QObject *pParent = nullptr);

    QIcon icon() const override;

    void initialize() override;

    AudioUnit* createInstance();

    QStringList environments() const;
    QVector<float> impulseResponse(const QString &env) const;

private:

    /// Map environment name to IR file name.
    QMap<QString, QString> m_irToFile;
};
