#ifndef AUDIOUNITSMANAGERWINDOW_H
#define AUDIOUNITSMANAGERWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QToolBox;

/**
 * This window shows the categorized list of all available audio units.
 */
class QMUSIC_VIEW_API AudioUnitsManagerWindow : public QDockWidget
{
    Q_OBJECT
public:

    AudioUnitsManagerWindow(QWidget *pParent = nullptr);

public slots:

    /**
     * Populate the view with the audio units.
     */
    void populate();

private:

    QToolBox *m_pToolBox;
};

#endif // AUDIOUNITSMANAGERWINDOW_H

