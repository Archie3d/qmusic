#ifndef SPECTRUMWINDOW_H
#define SPECTRUMWINDOW_H

#include <QDockWidget>
#include <QVector>
#include "ViewApi.h"

class QwtPlot;
class QwtPlotCurve;

class QMUSIC_VIEW_API SpectrumWindow : public QDockWidget
{
    Q_OBJECT
public:

    SpectrumWindow(QWidget *pParent = nullptr);

    void plotSpectrum();

    void reset();

    const QVector<float>& signal() const { return m_signal; }
    QVector<float>& signal() { return m_signal; }

public slots:

    void updateSpectrum();

signals:

    void spectrumUpdated();

private:

    void plotCurve(const QVector<float> &curve);

    void updateYAxisScale();

    /// Incoming signal.
    QVector<float> m_signal;

    QwtPlot *m_pPlot;
    QwtPlotCurve *m_pCurve;
    float m_yAxisScale;
};

#endif // SPECTRUMWINDOW_H

