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

#ifndef FFT_H
#define FFT_H

#include <complex>
#include <valarray>
#include "DspApi.h"

/**
 * @brief Implement fast Fourier transformation.
 * This class performs direct and inverse fast Fourier transform
 * including windowing.
 */
class QMUSIC_DSP_API Fft
{
public:

    /**
     * Window type.
     */
    typedef enum {
        Window_None,        ///< No window (square window).
        Window_Hann,        ///< Hann window.
        Window_Hamming,     ///< Hamming window.
        Window_Blackman     ///< Blackman window.
    } Window;

    /// Complex number based on float type.
    typedef std::complex<float> Complex;

    /// Array of complex numbers.
    typedef std::valarray<Complex> Array;

    /**
     * Perform direct FFT.
     * @param x Input data.
     */
    static void direct(Array &x, Window win = Window_None);

    /**
     * Perform inverse FFT.
     * @param x
     */
    static void inverse(Array &x);
};

#endif // FFT_H

