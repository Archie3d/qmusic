#ifndef VAONEPOLEFILTER_H
#define VAONEPOLEFILTER_H

#include "FrameworkApi.h"
#include "FilterAbstractImpl.h"

/**
 * @brief Virtual analogue single pole filter
 */
class QMUSIC_FRAMEWORK_API VAOnePoleFilter : public FilterAbstractImpl
{
public:

    enum Type {
        Type_LP,    /// Low-pass filter
        Type_HP     /// High-pass filter
    };

    VAOnePoleFilter();

    void reset() override;
    void update() override;
    double doFilter(double x) override;

    void setType(Type t) { m_type = t; }

    void setFeedback(double fb) { m_dFeedback = fb; }
    double getFeedbackOutput() const { return m_dFeedback; }

private:

    void resetFilter();
    void updateFilter();

    Type m_type;

    double m_dAlpha;
    double m_dBeta;
    double m_dZ1;
    double m_dGamma;
    double m_dDelta;
    double m_dEpsilon;
    double m_dA0;
    double m_dFeedback;

};

#endif // VAONEPOLEFILTER_H

