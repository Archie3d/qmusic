#ifndef AUDIOUNITSMANAGERWINDOW_H
#define AUDIOUNITSMANAGERWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QToolBox;

class QMUSIC_VIEW_API AudioUnitsManagerWindow : public QDockWidget
{
    Q_OBJECT
public:

    AudioUnitsManagerWindow(QWidget *pParent = nullptr);

public slots:

    void populate();

private:

    QToolBox *m_pToolBox;
};

#endif // AUDIOUNITSMANAGERWINDOW_H

