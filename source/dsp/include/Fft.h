#ifndef FFT_H
#define FFT_H

#include <complex>
#include <valarray>
#include "DspApi.h"

/**
 * Implement fast Fourier transformation
 */
class QMUSIC_DSP_API Fft
{
public:

    typedef std::complex<float> Complex;
    typedef std::valarray<Complex> Array;

    /**
     * Perform direct FFT.
     * @param x Input data.
     */
    static void direct(Array &x);

    /**
     * Perform inverse FFT.
     * @param x
     */
    static void inverse(Array &x);
};

#endif // FFT_H

