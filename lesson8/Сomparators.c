#include <assert.h>
#include <stdbool.h>
#include <math.h>

//======================================================================================================================

bool    DoubleComparison    (double x, double y);

bool    IsZero              (double x);

bool    IsNAN               (double x);

bool    IsINF               (double x);

bool    IsFinite            (double x);

//======================================================================================================================

const double EPSILON = 1e-6;

//======================================================================================================================

bool DoubleComparison(double x, double y) {
    if (IsNAN(x) || IsNAN(y)) {
        return IsNAN(x) && IsNAN(y);
    }
    if (IsINF(x) && IsINF(y)) {
        return true;
    }
    return (fabs(x - y)) < EPSILON ? true : false;
}

//----------------------------------------------------------------------------------------------------------------------

bool IsZero(double x) {
    assert(IsFinite(x));
    return DoubleComparison(x, 0);
}

//----------------------------------------------------------------------------------------------------------------------

bool IsNAN(double x) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

    return x != x;

#pragma GCC diagnostic pop
}

bool IsINF(double x) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

    return fabs(x) == INFINITY;

#pragma GCC diagnostic pop
}

bool IsFinite(double x) {
    if (IsNAN(x) || IsINF(x)) {
        return false;
    }
    return true;
}
