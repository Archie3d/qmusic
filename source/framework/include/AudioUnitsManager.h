#ifndef AUDIOUNITSMANAGER_H
#define AUDIOUNITSMANAGER_H

#include <QObject>
#include <QMap>
#include "FrameworkApi.h"

class QPluginLoader;
class AudioUnitPlugin;

class QMUSIC_FRAMEWORK_API AudioUnitsManager : public QObject
{
    Q_OBJECT
public:

    AudioUnitsManager(QObject *pParent = nullptr);
    ~AudioUnitsManager();

    QStringList categories() const;
    QList<AudioUnitPlugin*> audioUnitsInCategory(const QString &category) const;

    AudioUnitPlugin* audioUnitPluginByUid(const QString &uid) const;

public slots:

    void initialize();
    void cleanup();

signals:

    void initialized();

private:

    void load(const QString &absolutePath);
    void unload(const QString &uid);

    /// Map of plugin loaders to audio units UIDs.
    QMap<QString, QPluginLoader*> m_loaders;

    /// Map plugins by category.
    QMap<QString, QList<AudioUnitPlugin*> > m_pluginsPerCategory;
};

#endif // AUDIOUNITSMANAGER_H

