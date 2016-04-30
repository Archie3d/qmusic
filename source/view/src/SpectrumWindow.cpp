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

#include <QSplitter>
#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_engine.h>
#include "Fft.h"
#include "Application.h"
#include "AudioDevicesManager.h"
#include "AudioDevice.h"
#include "SpectrumWindow.h"

const QColor cWaveformColor(255, 144, 64);
const QColor cSpectrumColor(220, 220, 255);
const QColor cSpectrumFillColor(190, 190, 220, 128);
const QColor cAxisTitleColor("navy");
const float cMinFrequency(100.0f);
const float cMaxFrequency(22000.0f);
const float cDbScale(-320.0f);
const QFont cAxisFont("Verdana", 7);
const QFont cAxisTitleFont("Verdana", 7, QFont::Bold);

// Number of samples used to compute spectrum
const int cSignalSize(4096);

SpectrumWindow::SpectrumWindow(QWidget *pParent)
    : QDockWidget(pParent),
      m_signal(cSignalSize, 0.0f)
{
    setObjectName("spectrumWindow");
    setWindowTitle(tr("Audio output"));

    createWaveformPlot();
    createSpectrumPlot();

    QSplitter *pSplitter = new QSplitter(Qt::Horizontal);
    pSplitter->insertWidget(0, m_pWaveformPlot);
    pSplitter->insertWidget(1, m_pSpectrumPlot);

    setWidget(pSplitter);
}

SpectrumWindow::~SpectrumWindow()
{
    delete m_pWaveformPicker;
    delete m_pSpectrumPicker;
}

void SpectrumWindow::plotWaveform()
{
    plotWaveformCurve(m_signal);
}

void SpectrumWindow::plotSpectrum()
{
    Fft::Array input(m_signal.size());
    for (int i = 0; i < m_signal.size(); i++) {
        input[i] = qRound(m_signal.at(i) * 1e6) / 1e6;
    }

    Fft::direct(input, Fft::Window_Hann);

    QVector<float> curve;
    for (int i = 0; i < input.size() / 2; i++) {
        float v = std::abs(input[i]);
#if 1
        // Spectrum in dB.
        v = 20 * log(v / 16.0);
        v = qMax(v, cDbScale*1.2f); // Let the curve disappear
#endif
        curve.append(v);
    }

    plotSpectrumCurve(curve);
}

void SpectrumWindow::reset()
{
    m_pWaveformCurve->setSamples(QVector<QPointF>());
    m_pSpectrumCurve->setSamples(QVector<QPointF>());

    m_yAxisScale = 1.0;
    updateYAxisScale();
}

void SpectrumWindow::updateSpectrum()
{
    plotWaveform();
    plotSpectrum();
    emit spectrumUpdated();
}

void SpectrumWindow::createWaveformPlot()
{
    m_pWaveformPlot = new QwtPlot();
    m_pWaveformPlot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pWaveformPlot->setMinimumSize(0, 0);
    m_pWaveformPlot->setBaseSize(0, 0);
    m_pWaveformPlot->enableAxis(QwtPlot::xBottom, true);
    m_pWaveformPlot->enableAxis(QwtPlot::yLeft, true);
    m_pWaveformPlot->setAxisFont(QwtPlot::xBottom, cAxisFont);
    QwtText title(tr("Time, ms"));
    title.setFont(cAxisTitleFont);
    title.setColor(cAxisTitleColor);
    m_pWaveformPlot->setAxisTitle(QwtPlot::xBottom, title);
    m_pWaveformPlot->setAxisFont(QwtPlot::yLeft, cAxisFont);
    m_pWaveformPlot->setAxisAutoScale(QwtPlot::yLeft, false);
    m_pWaveformPlot->setAxisScale(QwtPlot::yLeft, -1.0, 1.0);
    m_pWaveformPlot->setCanvasBackground(QColor(32, 24, 16));
    dynamic_cast<QwtPlotCanvas*>(m_pWaveformPlot->canvas())->setFrameShadow(QFrame::Plain);

    m_pWaveformPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                          QwtPicker::CrossRubberBand,
                                          QwtPicker::AlwaysOn,
                                          m_pWaveformPlot->canvas());
    m_pWaveformPicker->setTrackerPen(QColor(255, 255, 128));

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->enableXMin(true);
    pGrid->setMajorPen(Qt::darkGray, 0, Qt::DotLine);
    pGrid->setMinorPen(Qt::gray, 0, Qt::DotLine);
    pGrid->attach(m_pWaveformPlot);

    QPen pen;
    pen.setColor(cWaveformColor);
    pen.setWidthF(1.5f);

    m_pWaveformCurve = new QwtPlotCurve();
    m_pWaveformCurve->setPen(pen);
    m_pWaveformCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_pWaveformCurve->attach(m_pWaveformPlot);
}

void SpectrumWindow::createSpectrumPlot()
{
    m_pSpectrumPlot = new QwtPlot();
    m_pSpectrumPlot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pSpectrumPlot->setMinimumSize(0, 0);
    m_pSpectrumPlot->setBaseSize(0, 0);
    m_pSpectrumPlot->enableAxis(QwtPlot::xBottom, true);
    m_pSpectrumPlot->enableAxis(QwtPlot::yLeft, true);
    QwtLogScaleEngine *pLogScale = new QwtLogScaleEngine();
    m_pSpectrumPlot->setAxisScaleEngine(QwtPlot::xBottom, pLogScale);
    m_pSpectrumPlot->setAxisAutoScale(QwtPlot::xBottom, false);
    m_pSpectrumPlot->setAxisScale(QwtPlot::xBottom, cMinFrequency, cMaxFrequency);
    m_pSpectrumPlot->setAxisFont(QwtPlot::xBottom, cAxisFont);
    m_pSpectrumPlot->setAxisFont(QwtPlot::yLeft, cAxisFont);
    m_pSpectrumPlot->setCanvasBackground(QColor(19, 19, 22));
    dynamic_cast<QwtPlotCanvas*>(m_pSpectrumPlot->canvas())->setFrameShadow(QFrame::Plain);

    m_pSpectrumPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                          QwtPicker::CrossRubberBand,
                                          QwtPicker::AlwaysOn,
                                          m_pSpectrumPlot->canvas());
    m_pSpectrumPicker->setTrackerPen(QColor(128, 255, 255));

    QwtText title(tr("Frequency, Hz"));
    title.setFont(cAxisTitleFont);
    title.setColor(cAxisTitleColor);
    m_pSpectrumPlot->setAxisTitle(QwtPlot::xBottom, title);
    m_pSpectrumPlot->setAxisAutoScale(QwtPlot::yLeft, false);
    //m_pSpectrumPlot->setAxisScale(QwtPlot::yLeft, 0.0, 1.0);
    m_pSpectrumPlot->setAxisScale(QwtPlot::yLeft, cDbScale, 0.0);
    m_yAxisScale = 1.0;

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->enableXMin(true);
    pGrid->setMajorPen(Qt::darkGray, 0, Qt::DotLine);
    pGrid->setMinorPen(Qt::gray, 0, Qt::DotLine);
    pGrid->attach(m_pSpectrumPlot);

    QPen pen;
    pen.setColor(cSpectrumColor);
    pen.setWidthF(1.0f);
    m_pSpectrumCurve = new QwtPlotCurve();
    m_pSpectrumCurve->setPen(pen);
    m_pSpectrumCurve->setBrush(cSpectrumFillColor);
    m_pSpectrumCurve->setBaseline(cDbScale - 50);   // Add offset to compensate plot margins
    m_pSpectrumCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_pSpectrumCurve->attach(m_pSpectrumPlot);
}

void SpectrumWindow::plotWaveformCurve(const QVector<float> &curve)
{
    QVector<QPointF> samples;

    float sampleRate = Application::instance()->audioDevicesManager()->audioOutputDevice()->openDeviceInfo().sampleRate;

    int i = 0;
    for (int i = 0; i < curve.size(); i++) {
        samples.append(QPointF(i * 1000.0f / sampleRate, curve.at(i)));
    }

    m_pWaveformCurve->setSamples(samples);
    m_pWaveformPlot->setAxisScale(QwtPlot::xBottom, 0.0, curve.size() * 1000.0f / sampleRate);
    m_pWaveformPlot->replot();
}

void SpectrumWindow::plotSpectrumCurve(const QVector<float> &curve)
{
    QVector<QPointF> samples;

    m_yAxisScale = 1.0f;
    float sampleRate = Application::instance()->audioDevicesManager()->audioOutputDevice()->openDeviceInfo().sampleRate;
    float df = sampleRate / 2.0f / curve.size();

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
    //m_pSpectrumPlot->setAxisScale(QwtPlot::yLeft, 0.0, qMin(100.0f, m_yAxisScale));
    m_pSpectrumPlot->setAxisScale(QwtPlot::yLeft, cDbScale, 0.0);
}
