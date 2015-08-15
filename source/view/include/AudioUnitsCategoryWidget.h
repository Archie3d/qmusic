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

#ifndef AUDIOUNITSCATEGORYWIDGET_H
#define AUDIOUNITSCATEGORYWIDGET_H

#include <QListView>
#include <QAbstractListModel>
#include "ViewApi.h"

class AudioUnitPlugin;
class AudioUnitsCategoryListModel;

/**
 * @brief List of audio units within the same category.
 */
class QMUSIC_VIEW_API AudioUnitsCategoryWidget : public QListView
{
    Q_OBJECT
public:

    AudioUnitsCategoryWidget(const QString &category, QWidget *pParent = nullptr);

private:

    AudioUnitsCategoryListModel *m_pListModel;
};

/**
 * @brief List model used to feed audio units within the same  category.
 */
class QMUSIC_VIEW_API AudioUnitsCategoryListModel : public QAbstractListModel
{
    Q_OBJECT
public:

    AudioUnitsCategoryListModel(const QString &category, QObject *pParent = nullptr);

    int rowCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;

    Qt::DropActions supportedDragActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;

    QString category() const { return m_category; }

private:

    QString m_category;
};

#endif // AUDIOUNITSCATEGORYWIDGET_H

