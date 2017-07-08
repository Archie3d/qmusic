#ifndef BIQUADFILTER_H
#define BIQUADFILTER_H

#include "DspApi.h"
#include "FilterAbstractImpl.h"

/**
 * @brief The biquad filter.
 *
 * This class implements the biquad filter.
 * The biquad filter can be configured in different way
 * to obtain LPF, HPF, BPF, notch and shelf filters.
 *
 * @see https://en.wikipedia.org/wiki/Digital_biquad_filter
 */
class QMUSIC_DSP_API BiquadFilter : public FilterAbstractImpl
{
public:

    /// Filter type
    enum Type {
        Type_LPF,       ///< Low-pass.
        Type_HPF,       ///< High-pass.
        Type_BPF,       ///< Band-pass.
        Type_Notch,     ///< Notch.
        Type_APF,       ///< All-pass
        Type_PeakingEQ, ///< Peaking equalizer filter.
        Type_LowShelf,  ///< Low-pass shelf.
        Type_HighShelf  ///< High-pass shelf.
    };

    explicit BiquadFilter(Type type = Type_LPF);

    void reset() override;
    void update() override;
    double doFilter(double x) override;

    Type type() const { return m_type; }
    void setType(Type type);

    double dBGain() const { return m_dBGain; }
    void setDBGain(double g);

    double qFactor() const { return m_q; }
    void setQFactor(double q);

private:

    // Recalculate filter coefficients according to
    // its type and cut-off frequency.
    void recalculate();

    /// Filter type.
    Type m_type;

    /// Gain in dB, used only for peaking and shelving filters.
    double m_dBGain;

    /// Filter's Q-factor, or
    /// bandwidth (in octaves) for the band-pass filter, or
    /// shelf slope.
    double m_q;


    // Filter coefficients.
    double m_a0;
    double m_a1;
    double m_a2;
    double m_b0;
    double m_b1;
    double m_b2;

    // Processing memory.
    double m_x_1;
    double m_x_2;
    double m_y_1;
    double m_y_2;
};


#endif // BIQUADFILTER_H
