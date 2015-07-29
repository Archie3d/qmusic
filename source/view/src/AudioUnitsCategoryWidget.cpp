#include <QMimeData>
#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitPlugin.h"
#include "../include/AudioUnitsCategoryWidget.h"

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

QMimeData* AudioUnitsCategoryListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *pMimeData = new QMimeData();
    QList<AudioUnitPlugin*> plugins = Application::instance()->audioUnitsManager()->audioUnitsInCategory(m_category);
    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            AudioUnitPlugin *pPlugin = plugins.at(index.row());
            pMimeData->setData(AudioUnitPlugin::MimeDataFormat, pPlugin->uid().toUtf8());
        }
    }
    return pMimeData;
}
