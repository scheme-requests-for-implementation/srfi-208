/* Copyright (C) 2020 Wolfgang Corcoran-Mathe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "nan.h"

/* We use type-punning through the following union type to
 * extract the unsigned representation of a double.
 */
union binary64 {
        uint64_t u;
        double   d;
};

const uint64_t sign_mask    = (uint64_t) 1 << 63;
const uint64_t quiet_mask   = (uint64_t) 1 << 51;
const uint64_t payload_mask = ((uint64_t) 1 << 51) - 1;

union binary32 {
        uint32_t u;
        float    f;
};

const uint32_t fsign_mask    = (uint32_t) 1 << 31;
const uint32_t fquiet_mask   = (uint32_t) 1 << 22;
const uint32_t fpayload_mask = ((uint32_t) 1 << 22) - 1;

/* Extract the unsigned representation of d. */
uint64_t bitsof(double d)
{
        union binary64 t;

        t.d = d;
        return t.u;
}

/* Warn if d is not a NaN.  In a real implementation, this would
 * create an error condition of some sort.
 */
void echknan(double d, const char *caller)
{
        if (fpclassify(d) != FP_NAN)
                fprintf(stderr, "%s: %f is not a NaN value.\n", caller, d);
}

/* (nan-negative?)  Return true if the sign bit of nan is set,
 * and false otherwise.
 */
bool nan_negative(double nan)
{
        echknan(nan, "nan_negative");
        return (bool) (bitsof(nan) & sign_mask);
}

/* (nan-quiet?)  Return true if the quiet bit of nan is set, and
 * false otherwise.
 */
bool nan_quiet(double nan)
{
        echknan(nan, "nan_quiet");
        return (bool) (bitsof(nan) & quiet_mask);
}

/* (nan-payload)  Return the payload of nan. */
unsigned long nan_payload(double nan)
{
        echknan(nan, "nan_payload");
        return bitsof(nan) & payload_mask;
}

/* (nan=?)  Return true if nan1 and nan2 have the same bit rep. */
bool nan_equal(double nan1, double nan2)
{
        echknan(nan1, "nan_equal");
        echknan(nan2, "nan_equal");
        return bitsof(nan1) == bitsof(nan2);
}

/* Return a NaN with characteristics determined by
 * the arguments.
 *
 * It is expected that a Scheme implementation of make-nan will
 * dispatch to this function or to fmake_nan() as determined by
 * make-nan's optional `float' argument.
 */
double make_nan(bool neg, bool quiet, unsigned long pay)
{
        union binary64 t;

        t.d = NAN;
        if (neg)
                t.u |= sign_mask;
        if (quiet)
                t.u |= quiet_mask;
        if (pay > payload_mask)
                fprintf(stderr, "make_nan: %lx: invalid payload\n", pay);
        else
                t.u |= pay;
        return t.d;
}

/* Single-float versions */

/* Extract the unsigned representation of f. */
uint32_t fbitsof(float f)
{
        union binary32 t;

        t.f = f;
        return t.u;
}

/* Warn if f is not a NaN.  In a real implementation, this would
 * create an error condition of some sort.
 */
void efchknan(float f, const char *caller)
{
        if (fpclassify(f) != FP_NAN)
                fprintf(stderr, "%s: %f is not a NaN value.\n", caller, f);
}

/* (single) (nan-negative?)  Return true if the sign bit of nan is set,
 * and false otherwise.
 */
bool fnan_negative(float nan)
{
        efchknan(nan, "nan_negative");
        return (bool) (fbitsof(nan) & fsign_mask);
}

/* (single) (nan-quiet?)  Return true if the quiet bit of nan is set,
 * and false otherwise.
 */
bool fnan_quiet(float nan)
{
        efchknan(nan, "nan_quiet");
        return (bool) (fbitsof(nan) & fquiet_mask);
}

/* (single) (nan-payload)  Return the payload of nan. */
unsigned int fnan_payload(float nan)
{
        efchknan(nan, "nan_payload");
        return fbitsof(nan) & fpayload_mask;
}

/* (single) (nan=?)  Return true if nan1 and nan2 have the same
 * bit representation.
 */
bool fnan_equal(float nan1, float nan2)
{
        efchknan(nan1, "nan_equal");
        efchknan(nan2, "nan_equal");
        return fbitsof(nan1) == fbitsof(nan2);
}

/* (single)  Return a NaN with characteristics determined by
 * the arguments.
 *
 * It is expected that a Scheme implementation of make-nan will
 * dispatch to this function or to make_nan() as determined by
 * make-nan's optional `float' argument.
 */
float fmake_nan(bool neg, bool quiet, unsigned int pay)
{
        union binary32 t;

        t.f = NAN;
        if (neg)
                t.u |= fsign_mask;
        if (quiet)
                t.u |= fquiet_mask;
        if (pay > fpayload_mask)
                fprintf(stderr, "fmake_nan: %x: invalid payload\n", pay);
        else
                t.u |= pay;
        return t.f;
}
