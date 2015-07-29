#ifndef IFILTER_H
#define IFILTER_H

#include "FrameworkApi.h"

/**
 * @brief Interface to a filter.
 */
class QMUSIC_FRAMEWORK_API IFilter
{
public:

    /**
     * Reset filter internal states and memory.
     */
    virtual void reset() = 0;

    /**
     * Update filter coefficients.
     */
    virtual void update() = 0;

    /**
     * Set filter sample rate.
     * @param sr
     */
    virtual void setSampleRate(double sr) = 0;

    /**
     * Set filter cut-off frequency.
     * @param f
     */
    virtual void setCutOffFrequency(double f) = 0;

    /**
     * Process a single sample.
     * @param x
     * @return
     */
    virtual double doFilter(double x) = 0;

    /// Destructor.
    virtual ~IFilter() {}
};

#endif // IFILTER_H

