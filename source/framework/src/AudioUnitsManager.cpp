#include <QDebug>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QCoreApplication>
#include "../include/AudioUnitPlugin.h"
#include "../include/AudioUnitsManager.h"

AudioUnitsManager::AudioUnitsManager(QObject *pParent)
    : QObject(pParent),
      m_loaders()
{
}

AudioUnitsManager::~AudioUnitsManager()
{
    cleanup();
}

QStringList AudioUnitsManager::categories() const
{
    return m_pluginsPerCategory.keys();
}

QList<AudioUnitPlugin*> AudioUnitsManager::audioUnitsInCategory(const QString &category) const
{
    return m_pluginsPerCategory.value(category);
}

AudioUnitPlugin* AudioUnitsManager::audioUnitPluginByUid(const QString &uid) const
{
    QPluginLoader *pLoader = m_loaders.value(uid);
    if (pLoader == nullptr) {
        return nullptr;
    }
    return dynamic_cast<AudioUnitPlugin*>(pLoader->instance());
}

void AudioUnitsManager::initialize()
{
    QDir path = QDir(qApp->applicationDirPath());

    // Compose list of plugin candidates
    QStringList list;
    foreach (QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.fileName().left(3) == "au-") {
            QString absolutePath = info.absoluteFilePath();
            if (QLibrary::isLibrary(absolutePath)) {
                list.append(absolutePath);
            }
        }
    }

    foreach (const QString &path, list) {
        load(path);
    }

    emit initialized();
}

void AudioUnitsManager::cleanup()
{
    foreach (const QString &uid, m_loaders.keys()) {
        unload(uid);
    }
}

void AudioUnitsManager::load(const QString &absolutePath)
{
    QPluginLoader *pLoader = new QPluginLoader(absolutePath);
    QJsonObject obj = pLoader->metaData().value("MetaData").toObject();

    QString uid = obj.value("uid").toString();
    QString name = obj.value("name").toString();
    QString category = obj.value("category").toString();
    QString version = obj.value("version").toString();

    if (uid.isEmpty() || name.isEmpty() || category.isEmpty() || version.isEmpty()) {
        qWarning() << "Audio unit plugin" << absolutePath << "metadata is missing";
        delete pLoader;
        return;
    }

    AudioUnitPlugin *pPlugin = qobject_cast<AudioUnitPlugin*>(pLoader->instance());
    if (pPlugin != nullptr) {
        pPlugin->setUid(uid);
        pPlugin->setName(name);
        pPlugin->setCategory(category);
        pPlugin->setVersion(version);

        m_loaders.insert(uid, pLoader);
        m_pluginsPerCategory[category].append(pPlugin);
        pPlugin->initialize();
        qDebug() << "Loaded audio unit" << uid << category << "/" << name << "/" << version;
    } else {
        qWarning() << "Unable to load audio unit plugin" << absolutePath;
        delete pLoader;
        return;
    }
}

void AudioUnitsManager::unload(const QString &uid)
{
    QPluginLoader *pLoader = m_loaders.value(uid);
    if (pLoader == nullptr) {
        return;
    }

    AudioUnitPlugin *pPlugin = qobject_cast<AudioUnitPlugin*>(pLoader->instance());
    if (pPlugin != nullptr) {
        pPlugin->cleanup();
    }

    m_pluginsPerCategory[pPlugin->category()].removeOne(pPlugin);
    if (pLoader->unload()) {
        qDebug() << "Audio unit" << uid << "has been unloaded";
        m_loaders.remove(uid);
        delete pLoader;
    } else {
        qWarning() << "Unable to unload plugin" << uid;
    }

}
