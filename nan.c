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

/* We use type-punning through the following union type to
 * extract the unsigned representation of a double.
 */
union ieee {
        uint64_t u;
        double   d;
};

uint64_t sign_mask    = (uint64_t) 1 << 63;
uint64_t quiet_mask   = (uint64_t) 1 << 51;
uint64_t payload_mask = ((uint64_t) 1 << 51) - 1;

/* Extract the unsigned representation of d. */
uint64_t bitsof(double d)
{
        union ieee t;

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
uint64_t nan_payload(double nan)
{
        echknan(nan, "nan_payload");
        return bitsof(nan) & payload_mask;
}
