#ifndef AUDIOUNITPROPERTIESWINDOW_H
#define AUDIOUNITPROPERTIESWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QtTreePropertyBrowser;
class QtVariantEditorFactory;
class AudioUnit;

class QMUSIC_VIEW_API AudioUnitPropertiesWindow : public QDockWidget
{
    Q_OBJECT
public:

    AudioUnitPropertiesWindow(QWidget *pParent = nullptr);
    void setAudioUnit(AudioUnit *pAudioUnit);
    void clear();

public slots:

    void handleAudioUnitSelected(AudioUnit *pAudioUnit);

private:
    QtTreePropertyBrowser *m_pPropertyBrowser;
    QtVariantEditorFactory *m_pPropertyFactory;
};

#endif // AUDIOUNITPROPERTIESWINDOW_H

