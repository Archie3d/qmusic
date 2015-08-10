#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"
#include "Logger.h"

class QTextEdit;

/**
 * Logging window.
 * This object captured log events from application logger and display then in a text box.
 * @see Logger
 */
class QMUSIC_VIEW_API LogWindow : public QDockWidget
{
    Q_OBJECT

public:

    LogWindow(QWidget *pParent = nullptr);

public slots:

    /**
     * @brief Clear log window.
     */
    void clear();

private slots:

    void onLogged(Logger::Level level, const QString &text);

private:

    QTextEdit *m_pLogText;
};

#endif // LOGWINDOW_H

