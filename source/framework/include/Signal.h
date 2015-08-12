#ifndef SIGNAL_H
#define SIGNAL_H

#include "FrameworkApi.h"

/**
 * Simple variant-type alternative for
 * fast signal processing.
 */
struct QMUSIC_FRAMEWORK_API Signal
{
    enum Type {
        Type_Invalid,
        Type_Int,
        Type_Float,
        Type_Bool
    };

    Signal()
        : type(Type_Invalid),
          asInt(0)
    {}

    Type type;
    union {
        int asInt;
        float asFloat;
        bool asBool;
    };
};

#endif // SIGNAL_H

