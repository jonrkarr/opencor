/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Compiler mathematical functions
//==============================================================================

#include "compilermath.h"

//==============================================================================

#include <cmath>
#include <cstdarg>
#include <cstdlib>

//==============================================================================

double compiler_fabs(double pNb)
{
    return ::fabs(pNb);
}

//==============================================================================

double compiler_log(double pNb)
{
    return ::log(pNb);
}

//==============================================================================

double compiler_exp(double pNb)
{
    return ::exp(pNb);
}

//==============================================================================

double compiler_floor(double pNb)
{
    return ::floor(pNb);
}

//==============================================================================

double compiler_ceil(double pNb)
{
    return ::ceil(pNb);
}

//==============================================================================

double compiler_factorial(double pNb)
{
    return tgamma(pNb+1.0);
    // Note: Γ(n) = (n-1)!...
}

//==============================================================================

double compiler_sin(double pNb)
{
    return ::sin(pNb);
}

//==============================================================================

double compiler_sinh(double pNb)
{
    return ::sinh(pNb);
}

//==============================================================================

double compiler_asin(double pNb)
{
    return ::asin(pNb);
}

//==============================================================================

double compiler_asinh(double pNb)
{
    return ::asinh(pNb);
}

//==============================================================================

double compiler_cos(double pNb)
{
    return ::cos(pNb);
}

//==============================================================================

double compiler_cosh(double pNb)
{
    return ::cosh(pNb);
}

//==============================================================================

double compiler_acos(double pNb)
{
    return ::acos(pNb);
}

//==============================================================================

double compiler_acosh(double pNb)
{
    return ::acosh(pNb);
}

//==============================================================================

double compiler_tan(double pNb)
{
    return ::tan(pNb);
}

//==============================================================================

double compiler_tanh(double pNb)
{
    return ::tanh(pNb);
}

//==============================================================================

double compiler_atan(double pNb)
{
    return ::atan(pNb);
}

//==============================================================================

double compiler_atanh(double pNb)
{
    return ::atanh(pNb);
}

//==============================================================================

double compiler_sec(double pNb)
{
    return 1.0/cos(pNb);
}

//==============================================================================

double compiler_sech(double pNb)
{
    return 1.0/cosh(pNb);
}

//==============================================================================

double compiler_asec(double pNb)
{
    return acos(1.0/pNb);
}

//==============================================================================

double compiler_asech(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return log(oneOverNb+sqrt(oneOverNb*oneOverNb-1.0));
}

//==============================================================================

double compiler_csc(double pNb)
{
    return 1.0/sin(pNb);
}

//==============================================================================

double compiler_csch(double pNb)
{
    return 1.0/sinh(pNb);
}

//==============================================================================

double compiler_acsc(double pNb)
{
    return asin(1.0/pNb);
}

//==============================================================================

double compiler_acsch(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return log(oneOverNb+sqrt(oneOverNb*oneOverNb+1.0));
}

//==============================================================================

double compiler_cot(double pNb)
{
    return 1.0/tan(pNb);
}

//==============================================================================

double compiler_coth(double pNb)
{
    return 1.0/tanh(pNb);
}

//==============================================================================

double compiler_acot(double pNb)
{
    return atan(1.0/pNb);
}

//==============================================================================

double compiler_acoth(double pNb)
{
    static const double HALF = 0.5;

    double oneOverNb = 1.0/pNb;

    return HALF*log((1.0+oneOverNb)/(1.0-oneOverNb));
}

//==============================================================================

double compiler_arbitrary_log(double pNb, double pBase)
{
    return std::log(pNb)/std::log(pBase);
}

//==============================================================================

double compiler_pow(double pNb1, double pNb2)
{
    return ::pow(pNb1, pNb2);
}

//==============================================================================

double compiler_multi_min(int pCount, ...)
{
    if (pCount == 0) {
        return strtod("NAN", nullptr);
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);
        double otherParameter;

        while (--pCount != 0) {
            otherParameter = va_arg(parameters, double);

            if (otherParameter < res) {
                res = otherParameter;
            }
        }
    va_end(parameters);

    return res;
}

//==============================================================================

double compiler_multi_max(int pCount, ...)
{
    if (pCount == 0) {
        return strtod("NAN", nullptr);
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);
        double otherParameter;

        while (--pCount != 0) {
            otherParameter = va_arg(parameters, double);

            if (otherParameter > res) {
                res = otherParameter;
            }
        }
    va_end(parameters);

    return res;
}

//==============================================================================

constexpr bool isEven(uint pNb)
{
    return pNb%2 == 0;
}

//==============================================================================

double compiler_gcd_pair(double pNb1, double pNb2)
{
    auto nb1 = static_cast<unsigned int>(std::fabs(pNb1));
    auto nb2 = static_cast<unsigned int>(std::fabs(pNb2));

    if (nb1 == 0) {
        return nb2;
    }

    if (nb2 == 0) {
        return nb1;
    }

    auto mult = 1U;

    while (isEven(nb1) && isEven(nb2)) {
        mult *= 2;

        nb1 /= 2;
        nb2 /= 2;
    }

    do {
        if (isEven(nb1)) {
            nb1 /= 2;
        } else if (isEven(nb2)) {
            nb2 /= 2;
        } else if (nb1 >= nb2) {
            nb1 = (nb1-nb2)/2;
        } else {
            nb2 = (nb2-nb1)/2;
        }
    } while (nb1 != 0);

    return mult*nb2;
}

//==============================================================================

double compiler_lcm_pair(double pNb1, double pNb2)
{
    return (pNb1*pNb2)/compiler_gcd_pair(pNb1, pNb2);
}

//==============================================================================

double compiler_gcd_multi(int pCount, ...)
{
    if (pCount == 0) {
        return 1.0;
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount != 0) {
            res = compiler_gcd_pair(res, va_arg(parameters, double));
        }
    va_end(parameters);

    return res;
}

//==============================================================================

double compiler_lcm_multi(int pCount, ...)
{
    if (pCount == 0) {
        return 1.0;
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount != 0) {
            res = compiler_lcm_pair(res, va_arg(parameters, double));
        }
    va_end(parameters);

    return res;
}

//==============================================================================
// End of file
//==============================================================================
