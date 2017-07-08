#include <qmath.h>
#include "BiquadFilter.h"

BiquadFilter::BiquadFilter(Type type)
    : FilterAbstractImpl(),
      m_type(type),
      m_dBGain(0.0),
      m_q(1.0)
{
    recalculate();
    m_x_1 = m_x_2 = m_y_1 = m_y_2 = 0.0;
}

void BiquadFilter::setType(Type t)
{
    m_type = t;
    recalculate();
}

void BiquadFilter::setDBGain(double g)
{
    m_dBGain = g;
    recalculate();
}

void BiquadFilter::setQFactor(double q)
{
    m_q = q;
    recalculate();
}

void BiquadFilter::reset()
{
    m_x_1 = m_x_2 = 0.0;
    m_y_1 = m_y_2 = 0.0;
}

void BiquadFilter::update()
{
    recalculate();
}

double BiquadFilter::doFilter(double x)
{
    double y = m_b0*x + m_b1*m_x_1 + m_b2*m_x_2
                      - m_a1*m_y_1 - m_a2*m_y_2;

    m_x_2 = m_x_1;
    m_x_1 = x;
    m_y_2 = m_y_1;
    m_y_1 = y;

    return y;
}

void BiquadFilter::recalculate()
{
    // @see http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

    double A = 0.0;
    if (m_type == Type_PeakingEQ || m_type == Type_LowShelf || m_type == Type_HighShelf) {
        A = sqrt(pow(10.0, m_dBGain/40.0));
    } else {
        A = sqrt(pow(10.0, m_dBGain/20.0));
    }
    double w0 = 2.0 * M_PI * cutOffFrequency() / sampleRate();

    double cos_w0 = cos(w0);
    double sin_w0 = sin(w0);
    double alpha = 0.0;

    switch (m_type)
    {
    case Type_LPF:
    case Type_HPF:
    case Type_APF:
        alpha = sin_w0 / (2.0 * m_q);
        break;
    case Type_BPF:
    case Type_Notch:
    case Type_PeakingEQ:
        alpha = sin_w0 * sinh(log(2.0)/2.0 * m_q * w0/sin_w0);
        break;
    case Type_LowShelf:
    case Type_HighShelf:
        alpha = sin_w0/2.0 * sqrt((A + 1.0/A) * (1/m_q - 1) + 2.0);
        break;
    default:
        Q_ASSERT(!"Should never get here");
    }

    switch (m_type) {
    case Type_LPF:
        m_b0 = (1.0 - cos_w0) / 2.0;
        m_b1 = 1.0 - cos_w0;
        m_b2 = (1.0 - cos_w0) / 2.0;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type_HPF:
        m_b0 = (1.0 + cos_w0) / 2.0;
        m_b1 = -(1.0 + cos_w0);
        m_b2 = (1.0 + cos_w0) / 2.0;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type_BPF:
        // Constant 0 dB peak gain
        m_b0 = alpha;
        m_b1 = 0.0;
        m_b2 = -alpha;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type_Notch:
        m_b0 = 1.0;
        m_b1 = -2.0 * cos_w0;
        m_b2 = 1.0f;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type_APF:
        m_b0 = 1.0 - alpha;
        m_b1 = -2.0 * cos_w0;
        m_b2 = 1.0 + alpha;
        m_a0 = 1.0 + alpha;
        m_a1 = -2.0 * cos_w0;
        m_a2 = 1.0 - alpha;
        break;
    case Type_PeakingEQ:
        m_b0 = 1.0 + alpha * A;
        m_b1 = -2.0 * cos_w0;
        m_b2 = 1.0 - alpha * A;
        m_a0 = 1.0 + alpha/A;
        m_a1 = -2.0*cos_w0;
        m_a2 = 1. - alpha/A;
        break;
    case Type_LowShelf:
        m_b0 = A*((A + 1.0) - (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha);
        m_b1 = 2.0*A*((A - 1.0) - (A + 1.0)*cos_w0);
        m_b2 = A*((A + 1.0) - (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha);
        m_a0 = (A + 1.0) + (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha;
        m_a1 = -2.0*((A - 1.0) + (A + 1.0)*cos_w0);
        m_a2 = (A + 1.0) + (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha;
        break;
    case Type_HighShelf:
        m_b0 = A*((A + 1.0) + (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha);
        m_b1 = -2.0*A*((A - 1.0) + (A + 1.0)*cos_w0);
        m_b2 = A*((A + 1.0) + (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha);
        m_a0 = (A + 1.0) - (A - 1.0)*cos_w0 + 2.0*sqrt(A)*alpha;
        m_a1 = 2.0*((A - 1) - (A + 1.0)*cos_w0);
        m_a2 = (A + 1.0) - (A - 1.0)*cos_w0 - 2.0*sqrt(A)*alpha;
        break;
    default:
        Q_ASSERT(!"Should never get here");
    }

    // Normalize the coefficients.
    m_a1 /= m_a0;
    m_a2 /= m_a0;
    m_b0 /= m_a0;
    m_b1 /= m_a0;
    m_b2 /= m_a0;
}
