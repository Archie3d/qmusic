#include <QToolBox>
#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitsCategoryWidget.h"
#include "AudioUnitsManagerWindow.h"

AudioUnitsManagerWindow::AudioUnitsManagerWindow(QWidget *pParent)
    : QDockWidget(tr("Audio Units"), pParent)
{
    setObjectName("AudioUnitsManagerWindow");

    m_pToolBox = new QToolBox();
    setWidget(m_pToolBox);

    connect(Application::instance()->audioUnitsManager(), SIGNAL(initialized()),
            this, SLOT(populate()));
}

void AudioUnitsManagerWindow::populate()
{
    QStringList categories = Application::instance()->audioUnitsManager()->categories();
    foreach (const QString &category, categories) {
        AudioUnitsCategoryWidget *pCategoryWidget = new AudioUnitsCategoryWidget(category);
        m_pToolBox->addItem(pCategoryWidget, category);
    }
}
