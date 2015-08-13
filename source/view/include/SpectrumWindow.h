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

    void reset();

private:

    void plotCurve(const QVector<float> &curve);

    void updateYAxisScale();

    QwtPlot *m_pPlot;
    QwtPlotCurve *m_pCurve;
    float m_yAxisScale;
};

#endif // SPECTRUMWINDOW_H

