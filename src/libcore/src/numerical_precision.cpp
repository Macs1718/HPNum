#include <cmath>
#include <complex>

#include "core/numerical_comparison"

template bool Numerical::almost_equal<float>( const float &a, const float &b,
                                              float eps );

template bool Numerical::almost_equal<double>( const double &a, const double &b,
                                               double eps );

template bool Numerical::almost_equal<std::complex<float>>(
    const std::complex<float> &a, const std::complex<float> &b, float eps );

template bool Numerical::almost_equal<std::complex<double>>(
    const std::complex<double> &a, const std::complex<double> &b, double eps );
