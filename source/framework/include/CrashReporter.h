/*
                          qmusic

    Copyright (C) 2016 Arthur Benilov,
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

#ifndef CRASHREPORTER_H
#define CRASHREPORTER_H

#include <qglobal.h>
#include "FrameworkApi.h"

/**
 * @brief Handle application crashes.
 * This class is used to intercept the application crash and dump the crash
 * log into a file.
 * Crash reporter implements the singleton design pattern.
 * NOTE: Currently implemented for Windows system only.
 */
class QMUSIC_FRAMEWORK_API CrashReporter
{
public:

    /**
     * Returns the instance of the crash reporter.
     * @return Pointer to the crash reporter singleton object.
     */
    static CrashReporter* instance();

private:

    Q_DISABLE_COPY(CrashReporter)

    CrashReporter();

    void installExceptionHandler();

    static CrashReporter *s_pInstance;
};

#endif // CRASHREPORTER_H

