#ifndef AUDIOUNITPROPERTIESWINDOW_H
#define AUDIOUNITPROPERTIESWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QtTreePropertyBrowser;
class QtVariantEditorFactory;
class AudioUnit;

/**
 * Window showing the list of properties of currently selected audio unit.
 */
class QMUSIC_VIEW_API AudioUnitPropertiesWindow : public QDockWidget
{
    Q_OBJECT
public:

    AudioUnitPropertiesWindow(QWidget *pParent = nullptr);

    /**
     * Display (and allow editing) properties of the audio unit.
     * @param pAudioUnit
     */
    void setAudioUnit(AudioUnit *pAudioUnit);

    /**
     * Clear the properties browser.
     */
    void clear();

public slots:

    void handleAudioUnitSelected(AudioUnit *pAudioUnit);

private:
    QtTreePropertyBrowser *m_pPropertyBrowser;
    QtVariantEditorFactory *m_pPropertyFactory;
};

#endif // AUDIOUNITPROPERTIESWINDOW_H

