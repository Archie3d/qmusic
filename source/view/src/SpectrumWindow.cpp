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

#include <QDebug>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include "Fft.h"
#include "Application.h"
#include "AudioDevicesManager.h"
#include "AudioDevice.h"
#include "SpectrumWindow.h"

const QColor cSpectrumColor("navy");
const float cMaxFrequency(20000.0f);
const QFont cAxisFont("Verdana", 7);

// Number of samples used to compute spectrum
const int cSignalSize(4096);

SpectrumWindow::SpectrumWindow(QWidget *pParent)
    : QDockWidget(pParent),
      m_signal(cSignalSize, 0.0f)
{
    setObjectName("spectrumWindow");
    setWindowTitle(tr("Spectrum"));

    m_pSpectrumPlot = new QwtPlot();
    m_pSpectrumPlot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pSpectrumPlot->setMinimumSize(0, 0);
    m_pSpectrumPlot->setBaseSize(0, 0);
    m_pSpectrumPlot->enableAxis(QwtPlot::xBottom, true);
    m_pSpectrumPlot->enableAxis(QwtPlot::yLeft, false);
    m_pSpectrumPlot->setAxisAutoScale(QwtPlot::xBottom, false);
    m_pSpectrumPlot->setAxisScale(QwtPlot::xBottom, 0.0, cMaxFrequency);
    m_pSpectrumPlot->setAxisFont(QwtPlot::xBottom, cAxisFont);
    m_pSpectrumPlot->setAxisAutoScale(QwtPlot::yLeft, false);
    m_pSpectrumPlot->setAxisScale(QwtPlot::yLeft, 0.0, 1.0);
    m_yAxisScale = 1.0;


    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->enableXMin(true);
    pGrid->setMajorPen(Qt::darkGray, 0, Qt::DotLine);
    pGrid->setMinorPen(Qt::gray, 0, Qt::DotLine);
    pGrid->attach(m_pSpectrumPlot);

    QPen pen;
    pen.setColor(cSpectrumColor);
    m_pSpectrumCurve = new QwtPlotCurve();
    m_pSpectrumCurve->setPen(pen);
    m_pSpectrumCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_pSpectrumCurve->attach(m_pSpectrumPlot);

    setWidget(m_pSpectrumPlot);
}

void SpectrumWindow::plotSpectrum()
{
    Fft::Array input(m_signal.size());
    for (int i = 0; i < m_signal.size(); i++) {
        input[i] = m_signal.at(i);
    }

    Fft::direct(input);

    QVector<float> curve;
    for (int i = 0; i < input.size() / 2; i++) {
        curve.append(std::abs(input[i]));
    }

    plotCurve(curve);
}

void SpectrumWindow::reset()
{
    m_pSpectrumCurve->setSamples(QVector<QPointF>());

    m_yAxisScale = 1.0;
    updateYAxisScale();
}

void SpectrumWindow::updateSpectrum()
{
    plotSpectrum();
    emit spectrumUpdated();
}

void SpectrumWindow::plotCurve(const QVector<float> &curve)
{
    QVector<QPointF> samples;

    float sampleRate = Application::instance()->audioDevicesManager()->audioOutputDevice()->openDeviceInfo().sampleRate;
    float df = sampleRate / 2.0f / curve.size();

    m_yAxisScale = 1.0f;

    int i = 0;
    float f = 0.0f;
    while (i < curve.size() && f <= cMaxFrequency) {
        samples.append(QPointF(f, curve.at(i)));
        m_yAxisScale = qMax(m_yAxisScale, curve.at(i));
        f = df * (++i);
    }

    m_pSpectrumCurve->setSamples(samples);

    updateYAxisScale();
    m_pSpectrumPlot->replot();
}

void SpectrumWindow::updateYAxisScale()
{    
    m_pSpectrumPlot->setAxisScale(QwtPlot::yLeft, 0.0, qMin(100.0f, m_yAxisScale));
}
