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

#include <QMimeData>
#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitPlugin.h"
#include "AudioUnitsCategoryWidget.h"

AudioUnitsCategoryWidget::AudioUnitsCategoryWidget(const QString &category, QWidget *pParent)
    : QListView(pParent)
{
    m_pListModel = new AudioUnitsCategoryListModel(category, this);
    setModel(m_pListModel);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
}

/*
 *  class AudioUnitsCategoryListModel implementation.
 */

AudioUnitsCategoryListModel::AudioUnitsCategoryListModel(const QString &category, QObject *pParent)
    : QAbstractListModel(pParent),
      m_category(category)
{
}

int AudioUnitsCategoryListModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Application::instance()->audioUnitsManager()->audioUnitsInCategory(m_category).count();
}

QVariant AudioUnitsCategoryListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    QList<AudioUnitPlugin*> plugins = Application::instance()->audioUnitsManager()->audioUnitsInCategory(m_category);
    if (index.row() >= plugins.count()) {
        return QVariant();
    }

    AudioUnitPlugin *pPlugin = plugins.at(index.row());
    Q_ASSERT(pPlugin != nullptr);

    switch (role) {
    case Qt::DisplayRole:
        return pPlugin->name();
    case Qt::DecorationRole:
        return pPlugin->icon();
    default:
        break;
    }

    return QVariant();
}

Qt::DropActions AudioUnitsCategoryListModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags AudioUnitsCategoryListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        defaultFlags |= Qt::ItemIsDragEnabled;
    }
    return defaultFlags;
}

QStringList AudioUnitsCategoryListModel::mimeTypes() const
{
    QStringList types;
    types << AudioUnitPlugin::MimeDataFormat;
    return types;
}

QMimeData* AudioUnitsCategoryListModel::mimeData(const QModelIndexList &indices) const
{
    QMimeData *pMimeData = new QMimeData();
    QList<AudioUnitPlugin*> plugins = Application::instance()->audioUnitsManager()->audioUnitsInCategory(m_category);
    for (const QModelIndex &index : indices) {
        if (index.isValid()) {
            AudioUnitPlugin *pPlugin = plugins.at(index.row());
            pMimeData->setData(AudioUnitPlugin::MimeDataFormat, pPlugin->uid().toUtf8());
        }
    }
    return pMimeData;
}
