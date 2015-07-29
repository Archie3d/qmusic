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

