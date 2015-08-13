#ifndef SPECTRUMWINDOW_H
#define SPECTRUMWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QwtPlot;
class QwtPlotCurve;

class QMUSIC_VIEW_API SpectrumWindow : public QDockWidget
{
    Q_OBJECT
public:

    SpectrumWindow(QWidget *pParent = nullptr);

    void plotSpectrum(const QVector<float> &signal);

private:

    void plotCurve(const QVector<float> &curve);

    QwtPlot *m_pPlot;
    QwtPlotCurve *m_pCurve;
};

#endif // SPECTRUMWINDOW_H

