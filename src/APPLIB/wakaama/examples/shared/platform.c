/*******************************************************************************
 *
 * Copyright (c) 2013, 2014, 2015 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - xiny_initial API and implementation
 *******************************************************************************/

#include "xiny_liblwm2m.h"
#include "low_power.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#ifndef xiny_LWM2M_MEMORY_TRACE

void * xiny_lwm2m_malloc(size_t s)
{
    return malloc(s);
}

void xiny_lwm2m_free(void * p)
{
    return free(p);
}

char * xiny_lwm2m_strdup(const char * str)
{
    return strdup(str);
}

#endif

int xiny_lwm2m_strncmp(const char * s1,
                     const char * s2,
                     size_t n)
{
    return strncmp(s1, s2, n);
}

time_t xiny_lwm2m_gettime(void)
{
    return xy_rtc_get_sec(0);

}

void xiny_lwm2m_printf(const char * format, ...)
{
    va_list ap;

    va_start(ap, format);

    vfprintf(stderr, format, ap);

    va_end(ap);
}