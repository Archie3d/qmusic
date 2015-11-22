/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

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

    void plotWaveform();
    void plotSpectrum();

    void reset();

    const QVector<float>& signal() const { return m_signal; }
    QVector<float>& signal() { return m_signal; }

public slots:

    void updateSpectrum();

signals:

    void spectrumUpdated();

private:

    void createWaveformPlot();
    void createSpectrumPlot();

    void plotWaveformCurve(const QVector<float> &curve);
    void plotSpectrumCurve(const QVector<float> &curve);

    void updateYAxisScale();

    /// Incoming signal.
    QVector<float> m_signal;

    QwtPlot *m_pWaveformPlot;
    QwtPlotCurve *m_pWaveformCurve;

    QwtPlot *m_pSpectrumPlot;
    QwtPlotCurve *m_pSpectrumCurve;
    float m_yAxisScale;
};

#endif // SPECTRUMWINDOW_H

