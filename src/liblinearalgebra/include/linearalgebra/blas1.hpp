// Copyright 2017 Dr. Xavier JUVIGNY

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =========================================================================
#ifndef _LINEARALGEBRA_BLAS1_HPP_
# define _LINEARALGEBRA_BLAS1_HPP_
# include <complex>

# if defined(USE_MKL)
extern "C" {
#   include <mkl_cblas.h>
}
#   define LINALG_CCOMPLEX void*
#   define LINALG_ZCOMPLEX void*
#   define LINALG_CRET     void*
#   define LINALG_ZRET     void*
# elif defined(USE_OPENBLAS)
#   include <cblas.h>
#   define LINALG_CCOMPLEX float*
#   define LINALG_ZCOMPLEX double*
#   define LINALG_CRET     openblas_complex_float*
#   define LINALG_ZRET     openblas_complex_double*
# else
extern "C" {
#     include <cblas.h>
}
#   define LINALG_CCOMPLEX void*
#   define LINALG_ZCOMPLEX void*
#   define LINALG_CRET     void*
#   define LINALG_ZRET     void*
# endif

namespace Blas
{
    // ============================== dot routine ==============================
    inline float
    dot(int N, const float *X, const float *Y, int incX = 1, int incY = 1)
    {
	return cblas_sdot(N, X, incX, Y, incY);
    }

    inline double
    dot(int N, const double *X, const double *Y, int incX = 1, int incY = 1)
    {
	return cblas_ddot(N, X, incX, Y, incY);
    }

    inline std::complex<float>
    dot(int N, const std::complex<float> *X, const std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	std::complex<float> z;
	cblas_cdotc_sub(N, (LINALG_CCOMPLEX)X, incX, (LINALG_CCOMPLEX)Y, incY,
			(LINALG_CRET)&z);
	return z;
    }

    inline std::complex<double>
    dot(int N, const std::complex<double> *X, const std::complex<double> *Y,
        int incX = 1, int incY = 1)
    {
	std::complex<double> z;
	cblas_zdotc_sub(N, (LINALG_ZCOMPLEX)X, incX, (LINALG_ZCOMPLEX)Y, incY,
			(LINALG_ZRET)&z);
	return z;
    }
    // ============================== dotu routine ==============================
    inline std::complex<float>
    dotu(int N, const std::complex<float> *X, const std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	std::complex<float> z;
	cblas_cdotu_sub(N, (LINALG_CCOMPLEX)X, incX, (LINALG_CCOMPLEX)Y, incY,
			(LINALG_CRET)&z);
	return z;
    }
    // ..................................................
    inline std::complex<double>
    dotu(int N, const std::complex<double> *X, const std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	std::complex<double> z;
	cblas_zdotu_sub(N, (LINALG_ZCOMPLEX)X, incX, (LINALG_ZCOMPLEX)Y, incY,
			(LINALG_ZRET)&z);
	return z;
    }
    // ========================== Euclidian norm routine ======================
    inline float nrm2(int N, const float *X, int incx = 1)
    {
	return cblas_snrm2(N, X, incx);
    }
    // ..................................................
    inline double nrm2(int N, const double *X, int incX = 1)
    {
	return cblas_dnrm2(N, X, incX);
    }
    // ..................................................
    inline float
    nrm2(int N, const std::complex<float> *X, int incX = 1)
    {
	return cblas_scnrm2(N, (LINALG_CCOMPLEX)X, incX);
    }
    // ..................................................
    inline double
    nrm2(int N, const std::complex<double> *X, int incX = 1)
    {
	return cblas_dznrm2(N, (LINALG_ZCOMPLEX)X, incX);
    }
    // ========================== L1 norm routine =============================
    inline float asum(int N, const float *X, int incX = 1)
    {
	return cblas_sasum(N, X, incX);
    }
    // ..................................................
    inline double asum(int N, const double *X, int incX = 1)
    {
	return cblas_dasum(N, X, incX);
    }
    // ..................................................
    inline float
    asum(int N, const std::complex<float> *X, int incX = 1 )
    {
	return cblas_scasum(N, (LINALG_CCOMPLEX)X, incX);
    }
    // ..................................................
    inline double
    asum(int N, const std::complex<double> *X, int incX = 1 )
    {
	return cblas_dzasum(N, (LINALG_ZCOMPLEX)X, incX);
    }
    // =============== Max value searching algorithm ==========================
    inline size_t
    iamax(int N, const float *X, int incX = 1)
    {
	return size_t(cblas_isamax(N, X, incX));
    }
    // ..................................................
    inline size_t
    iamax(int N, const double *X, int incX = 1)
    {
	return size_t(cblas_idamax(N, X, incX));
    }
    // ..................................................
    inline size_t
    iamax(int N, const std::complex<float> *X, int incX = 1)
    {
	return size_t(cblas_icamax(N, (LINALG_CCOMPLEX)X, incX));
    }
    // ..................................................
    inline size_t
    iamax(int N, const std::complex<double> *X, int incX = 1)
    {
	return size_t(cblas_izamax(N, (LINALG_ZCOMPLEX)X, incX));
    }
    // ======================= Swap routine ===================================
    inline void
    swap(int N, float *X, float *Y, int incX = 1, int incY = 1)
    {
	cblas_sswap(N, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    swap(int N, double *X, double *Y, int incX = 1, int incY = 1)
    {
	cblas_dswap(N, X, incX, Y, incY);
    }
    // ------------------------------------------------------------------------
    inline void
    swap(int N, std::complex<float> *X, std::complex<float> *Y, int incX = 1, int incY = 1 )
    {
	cblas_cswap(N, (LINALG_CCOMPLEX)X, incX, (LINALG_CCOMPLEX)Y, incY);
    }
    // ------------------------------------------------------------------------
    inline void
    swap(int N, std::complex<double> *X, std::complex<double> *Y, int incX = 1, int incY = 1 )
    {
	cblas_zswap(N, (LINALG_ZCOMPLEX)X, incX, (LINALG_ZCOMPLEX)Y, incY);
    }
    // ======================== Copy routine ==================================
    inline void
    copy(int N, const float *X, float *Y, int incX = 1, int incY = 1)
    {
	cblas_scopy(N, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    copy(int N, const double *X, double *Y, int incX = 1, int incY = 1)
    {
	cblas_dcopy(N, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    copy(int N, const std::complex<float> *X, std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	cblas_ccopy(N, (LINALG_CCOMPLEX)X, incX, (LINALG_CCOMPLEX)Y, incY);
    }
    // ........................................................................
    inline void
    copy(int N, const std::complex<double> *X, std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	cblas_zcopy(N, (LINALG_ZCOMPLEX)X, incX, (LINALG_ZCOMPLEX)Y, incY);
    }
    // ======================== axpy routine ==================================
    inline void
    axpy(int N, float alpha, const float *X, float *Y, int incX = 1, int incY = 1)
    {
	cblas_saxpy(N, alpha, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    axpy(int N, double alpha, const double *X, double *Y, int incX = 1, int incY = 1)
    {
	cblas_daxpy(N, alpha, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    axpy(int N, std::complex<float> alpha,
	 const std::complex<float> *X, std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	cblas_caxpy(N, (const LINALG_CCOMPLEX)&alpha, (const LINALG_CCOMPLEX)X, incX,
		    (LINALG_CCOMPLEX)Y, incY);
    }
    // ........................................................................
    inline void
    axpy(int N, std::complex<double> alpha, const std::complex<double> *X,
	 std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	cblas_zaxpy(N, (const LINALG_ZCOMPLEX)&alpha, (const LINALG_ZCOMPLEX)X, incX,
		    (LINALG_ZCOMPLEX)Y, incY);
    }
    // ========================== axpby routine ===============================
#   ifdef USEMKL2013
    inline void
    axpby(int N, float alpha, const float *X, float beta, float *Y, int incX = 1, int incY = 1)
    {
	cblas_saxpby(N, alpha, X, incX, beta, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, double alpha, const double *X, double beta, double *Y, int incX = 1, int incY = 1)
    {
	cblas_daxpby(N, alpha, X, incX, beta, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<float> alpha, const std::complex<float> *X,
	  std::complex<float> beta, std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	cblas_caxpby(N, (void *)&alpha, (void *)X, incX, (void *)&beta, (void *)Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<double> alpha, const std::complex<double> *X,
	  std::complex<double> beta, std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	cblas_zaxpby(N, (void *)&alpha, (void *)X, incX, (void *)&beta, (void *)Y, incY);
    }
#   elif defined(USEOPENBLAS)
    inline void
    axpby(int N, float alpha, const float *X, float beta, float *Y, int incX = 1, int incY = 1)
    {
	cblas_saxpby(N, alpha, X, incX, beta, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, double alpha, const double *X, double beta, double *Y, int incX = 1, int incY = 1)
    {
	cblas_daxpby(N, alpha, X, incX, beta, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<float> alpha, const std::complex<float> *X,
	  std::complex<float> beta, std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	cblas_caxpby(N, (const float *)&alpha, (const float *)X, incX,
		     (const float *)&beta, (float *)Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<double> alpha, const std::complex<double> *X,
	  std::complex<double> beta, std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	cblas_zaxpby(N, (const double *)&alpha, (const double *)X, incX,
		     (const double *)&beta, (double *)Y, incY);
    }
#   elif defined(USEATLAS)
    inline void
    axpby(int N, float alpha, const float *X, float beta, float *Y, int incX = 1, int incY = 1)
    {
	catlas_saxpby(N, alpha, X, incX, beta, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, double alpha, const double *X, double beta, double *Y, int incX = 1, int incY = 1)
    {
	catlas_daxpby(N, alpha, X, incX, beta, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<float> alpha, const std::complex<float> *X,
	  std::complex<float> beta, std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	catlas_caxpby(N, (void *)&alpha, (void *)X, incX, (void *)&beta, (void *)Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<double> alpha, const std::complex<double> *X,
	  std::complex<double> beta, std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	catlas_zaxpby(N, (void *)&alpha, (void *)X, incX, (void *)&beta, (void *)Y, incY);
    }
#   else
    inline void
    axpby(int N, float alpha, const float *X, float beta, float *Y, int incX = 1, int incY = 1)
    {
	cblas_sscal(N, beta, Y, incY);
	cblas_saxpy(N, alpha, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, double alpha, const double *X, double beta, double *Y, int incX = 1, int incY = 1)
    {
	cblas_dscal(N, beta, Y, incY);
	cblas_daxpy(N, alpha, X, incX, Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<float> alpha, const std::complex<float> *X,
	  std::complex<float> beta, std::complex<float> *Y, int incX = 1, int incY = 1)
    {
	cblas_cscal(N, (void *)&beta, (void *)Y, incY);
	cblas_caxpy(N, (void *)&alpha, (void *)X, incX, (void *)Y, incY);
    }
    // ........................................................................
    inline void
    axpby(int N, std::complex<double> alpha, const std::complex<double> *X,
	  std::complex<double> beta, std::complex<double> *Y, int incX = 1, int incY = 1)
    {
	cblas_zscal(N, (void *)&beta, (void *)Y, incY);
	cblas_zaxpy(N, (void *)&alpha, (void *)X, incX, (void *)Y, incY);
    }
#   endif
    // =================== Perform rotation of points in the plane ============
    inline void
    rot(int N, float *X, float *Y, float c, float s, int incX = 1, int incY = 1)
    {
	cblas_srot(N, X, incX, Y, incY, c, s);
    }
    // ........................................................................
    inline void
    rot(int N, double *X, double *Y, double c, double s, int incX = 1, int incY = 1 )
    {
	cblas_drot(N, X, incX, Y, incY, c, s);
    }
    // ........................................................................
#   if defined(BLAS_COMPLEX_ROT)
    inline void
    rot(int N, std::complex<float> *X, std::complex<float> *Y, float c, float s,
	int incX = 1, int incY = 1 )
    {
	cblas_csrot(N, (LINALG_CCOMPLEX)X, incX, (LINALG_CCOMPLEX)Y, incY, c, s);
    }
    // ........................................................................
    inline void
    rot(int N, std::complex<double> *X, std::complex<double> *Y, double c, double s,
	int incX = 1, int incY = 1 )
    {
	cblas_zdrot(N, (LINALG_ZCOMPLEX)X, incX, (LINALG_ZCOMPLEX)Y, incY, c, s);
    }
#   endif
    // ================== Compute parameters for a givens rotation ============
    inline void rotg(float &a, float &b, float &c, float &s)
    {
	cblas_srotg(&a, &b, &c, &s);
    }
    // ------------------------------------------------------------------------
    inline void rotg(double &a, double &b, double &c, double &s)
    {
	cblas_drotg(&a, &b, &c, &s);
    }
    // ======================== Product of a vector by a scalar ===============
    inline void
    scal(int N, float alpha, float *X, int incX = 1)
    {
	cblas_sscal(N, alpha, X, incX);
    }
    // ........................................................................
    inline void
    scal(int N, double alpha, double *X, int incX = 1 )
    {
	cblas_dscal(N, alpha, X, incX);
    }
    // ........................................................................
    inline void
    scal(int N, std::complex<float> alpha, std::complex<float> *X, int incX = 1)
    {
	cblas_cscal(N, (LINALG_CCOMPLEX)&alpha, (LINALG_CCOMPLEX)X, incX);
    }
    // ........................................................................
    inline void
    scal(int N, std::complex<double> alpha, std::complex<double> *X, int incX = 1)
    {
	cblas_zscal(N, (LINALG_ZCOMPLEX)&alpha, (LINALG_ZCOMPLEX)X, incX);
    }
    // ........................................................................
    inline void
    scal(int N, float alpha, std::complex<float> *X, int incX = 1)
    {
	cblas_csscal(N, alpha, (LINALG_CCOMPLEX)X, incX);
    }
    // ..................................................
    inline void
    scal(int N, double alpha, std::complex<double> *X, int incX = 1)
    {
	cblas_zdscal(N, alpha, (LINALG_ZCOMPLEX)X, incX);
    }
}
# undef LINALG_CCOMPLEX
# undef LINALG_ZCOMPLEX
# undef LINALG_CRET
# undef LINALG_ZRET

#endif
