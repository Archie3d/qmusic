/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#include <QDebug>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QCoreApplication>
#include "AudioUnitPlugin.h"
#include "AudioUnitsManager.h"

const QString cPluginPrefix =
#if defined(Q_OS_OSX) || defined(Q_OS_LINUX)
    "libau-";
#elif defined(Q_OS_WIN)
    "au-";
#else
    "au-";
#endif

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
    for (QFileInfo info : path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.fileName().left(cPluginPrefix.length()) == cPluginPrefix) {
            QString absolutePath = info.absoluteFilePath();
            if (QLibrary::isLibrary(absolutePath)) {
                list.append(absolutePath);
            }
        }
    }

    for (const QString &path : list) {
        load(path);
    }

    emit initialized();
}

void AudioUnitsManager::cleanup()
{
    for (const QString &uid : m_loaders.keys()) {
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
