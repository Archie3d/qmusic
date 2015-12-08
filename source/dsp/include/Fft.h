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
 * Implement fast Fourier transformation
 */
class QMUSIC_DSP_API Fft
{
public:

    typedef enum {
        Window_None,
        Window_Hann,
        Window_Hamming,
        Window_Blackman
    } Window;

    typedef std::complex<float> Complex;
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

