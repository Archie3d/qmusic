#include <QDebug>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include "Fft.h"
#include "Application.h"
#include "AudioDevice.h"
#include "SpectrumWindow.h"

const QColor cSpectrumColor("navy");

SpectrumWindow::SpectrumWindow(QWidget *pParent)
    : QDockWidget(pParent)
{
    setObjectName("spectrumWindow");
    setWindowTitle(tr("Spectrum"));

    m_pPlot = new QwtPlot();
    m_pPlot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pPlot->setMinimumSize(0, 0);
    m_pPlot->setBaseSize(0, 0);
    m_pPlot->enableAxis(QwtPlot::yLeft, false);

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->enableXMin(true);
    pGrid->setMajorPen(Qt::darkGray, 0, Qt::DotLine);
    pGrid->setMinorPen(Qt::gray, 0, Qt::DotLine);
    pGrid->attach(m_pPlot);

    QPen pen;
    pen.setColor(cSpectrumColor);
    m_pCurve = new QwtPlotCurve();
    m_pCurve->setPen(pen);
    m_pCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_pCurve->attach(m_pPlot);

    setWidget(m_pPlot);
}

void SpectrumWindow::plotSpectrum(const QVector<float> &signal)
{
    Fft::Array input(signal.size());
    for (int i = 0; i < signal.size(); i++) {
        input[i] = signal.at(i);
    }

    Fft::direct(input);

    QVector<float> curve;
    for (int i = 0; i < input.size() / 2; i++) {
        curve.append(std::abs(input[i]));
    }

    plotCurve(curve);
}

void SpectrumWindow::plotCurve(const QVector<float> &curve)
{
    QVector<QPointF> samples;

    float sampleRate = Application::instance()->audioOutputDevice()->openDeviceInfo().sampleRate;
    float df = sampleRate / 2.0f / curve.size();

    for (int i = 0; i < curve.size(); i++) {
        samples.append(QPointF(df * i, curve.at(i)));
    }

    m_pCurve->setSamples(samples);

    m_pPlot->replot();
}
