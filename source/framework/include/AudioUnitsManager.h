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

#ifndef AUDIOUNITSMANAGER_H
#define AUDIOUNITSMANAGER_H

#include <QObject>
#include <QMap>
#include "FrameworkApi.h"

class QPluginLoader;
class AudioUnitPlugin;

/**
 * This class manages loading and unloading audio unit plugins.
 */
class QMUSIC_FRAMEWORK_API AudioUnitsManager : public QObject
{
    Q_OBJECT
public:

    AudioUnitsManager(QObject *pParent = nullptr);
    ~AudioUnitsManager();

    /**
     * Returns the list of all plugins categories.
     * @return
     */
    QStringList categories() const;

    /**
     * Returns the list of audio unit plugins within given category.
     * @param category
     * @return
     */
    QList<AudioUnitPlugin*> audioUnitsInCategory(const QString &category) const;

    /**
     * Returns audio unit plugin by its UID.
     * @param uid
     * @return
     */
    AudioUnitPlugin* audioUnitPluginByUid(const QString &uid) const;

public slots:

    /**
     * Initialize all plugins.
     */
    void initialize();

    /**
     * Clean-up all plugins.
     */
    void cleanup();

signals:

    /**
     * Signal used to notfy plugin initialization.
     */
    void initialized();

private:

    /**
     * Load plugin from file.
     * @param absolutePath Plugin file absolute path.
     */
    void load(const QString &absolutePath);

    /**
     * Unload plugin.
     * @param uid Plugin's UID.
     */
    void unload(const QString &uid);

    /// Map of plugin loaders to audio units UIDs.
    QMap<QString, QPluginLoader*> m_loaders;

    /// Map plugins by category.
    QMap<QString, QList<AudioUnitPlugin*> > m_pluginsPerCategory;
};

#endif // AUDIOUNITSMANAGER_H

