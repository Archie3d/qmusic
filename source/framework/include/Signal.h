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

