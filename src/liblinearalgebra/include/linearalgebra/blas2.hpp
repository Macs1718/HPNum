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
// ========================================================================
// ==           Blas encapsulation for template compatibility            ==
// ========================================================================
#ifndef _LINEARALGEBRA_BLAS2_HPP_
#define _LINEARALGEBRA_BLAS2_HPP_
#include <complex>

#if defined( USE_MKL )
extern "C" {
#include <mkl_cblas.h>
#define LINALG_CCOMPLEX void *
#define LINALG_ZCOMPLEX void *
#define LINALG_CRET void *
#define LINALG_ZRET void *
}
#elif defined( USE_OPENBLAS )
#include <cblas.h>
#define LINALG_CCOMPLEX float *
#define LINALG_ZCOMPLEX double *
#define LINALG_CRET openblas_complex_float *
#define LINALG_ZRET openblas_complex_double *
#else
extern "C" {
#include <cblas.h>
}
#define LINALG_CCOMPLEX void *
#define LINALG_ZCOMPLEX void *
#define LINALG_CRET void *
#define LINALG_ZRET void *
#endif

namespace Blas {
    // ================ General Matrix product with vector ====================

    /**
     * @brief      General Matrix-vector product
     *
     * @param[in]  M      { parameter_description }
     * @param[in]  N      { parameter_description }
     * @param[in]  A      { parameter_description }
     * @param[in]  X      { parameter_description }
     * @param      Y      { parameter_description }
     * @param[in]  alpha  The alpha
     * @param[in]  beta   The beta
     * @param[in]  trA    The tr a
     * @param[in]  ldA    The ld a
     * @param[in]  incX   The increment of x
     * @param[in]  incY   The increment y
     * @param[in]  order  The order
     * @param[in]  toto  The toto
     */
    inline void gemv( int M, int N, const float *A, const float *X, float *Y,
                      float alpha = 1., float beta = 0.,
                      CBLAS_TRANSPOSE trA = CblasNoTrans, int ldA = -1,
                      int incX = 1, int incY = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? ( CblasColMajor == order ? M : N ) : ldA );
        cblas_sgemv( order, trA, M, N, alpha, A, ldA, X, incX, beta, Y, incY );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      { function_description }
     *
     * @param[in]  M      { parameter_description }
     * @param[in]  N      { parameter_description }
     * @param[in]  A      { parameter_description }
     * @param[in]  X      { parameter_description }
     * @param      Y      { parameter_description }
     * @param[in]  alpha  The alpha
     * @param[in]  beta   The beta
     * @param[in]  trA    The tr a
     * @param[in]  ldA    The ld a
     * @param[in]  incX   The increment of x
     * @param[in]  incY   The increment of y
     * @param[in]  order  The order
     */
    inline void gemv( int M, int N, const double *A, const double *X, double *Y,
                      double alpha = 1., double beta = 0.,
                      CBLAS_TRANSPOSE trA = CblasNoTrans, int ldA = -1,
                      int incX = 1, int incY = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? ( CblasColMajor == order ? M : N ) : ldA );
        cblas_dgemv( order, trA, M, N, alpha, A, ldA, X, incX, beta, Y, incY );
    }
    // ------------------------------------------------------------------------
    inline void gemv( int M, int N, const std::complex<float> *A,
                      const std::complex<float> *X, std::complex<float> *Y,
                      std::complex<float> alpha = 1.,
                      std::complex<float> beta  = 0.,
                      CBLAS_TRANSPOSE trA = CblasNoTrans, int ldA = -1,
                      int incX = 1, int incY = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? ( CblasColMajor == order ? M : N ) : ldA );
        cblas_cgemv( order, trA, M, N, (const LINALG_CCOMPLEX)&alpha,
                     (const LINALG_CCOMPLEX)A, ldA, (const LINALG_CCOMPLEX)X,
                     incX, (const LINALG_CCOMPLEX)&beta, (LINALG_CCOMPLEX)Y,
                     incY );
    }
    // ------------------------------------------------------------------------
    inline void gemv( int M, int N, const std::complex<double> *A,
                      const std::complex<double> *X, std::complex<double> *Y,
                      std::complex<double> alpha = 1.,
                      std::complex<double> beta  = 0.,
                      CBLAS_TRANSPOSE trA = CblasNoTrans, int ldA = -1,
                      int incX = 1, int incY = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? ( CblasColMajor == order ? M : N ) : ldA );
        cblas_zgemv( order, trA, M, N, (const LINALG_ZCOMPLEX)&alpha,
                     (const LINALG_ZCOMPLEX)A, ldA, (const LINALG_ZCOMPLEX)X,
                     incX, (const LINALG_ZCOMPLEX)&beta, (LINALG_ZCOMPLEX)Y,
                     incY );
    }
    // ==================== Band matrix - vector product ======================
    inline void gbmv( int M, int N, int KL, int KU, const float *A,
                      const float *x, float *y, float alpha = 1.,
                      float beta = 0., CBLAS_TRANSPOSE trA = CblasNoTrans,
                      int ldA = -1, int incX = 1, int incY = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? KL + KU + 1 : ldA );
        cblas_sgbmv( order, trA, M, N, KL, KU, alpha, A, ldA, x, incX, beta, y,
                     incY );
    }
    // ------------------------------------------------------------------------
    inline void gbmv( int M, int N, int KL, int KU, const double *A,
                      const double *x, double *y, double alpha = 1.,
                      double beta = 0., CBLAS_TRANSPOSE trA = CblasNoTrans,
                      int ldA = -1, int incX = 1, int incY = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? KL + KU + 1 : ldA );
        cblas_dgbmv( order, trA, M, N, KL, KU, alpha, A, ldA, x, incX, beta, y,
                     incY );
    }
    // ------------------------------------------------------------------------
    inline void
    gbmv( int M, int N, int KL, int KU, const std::complex<float> *A,
          const std::complex<float> *x, std::complex<float> *y,
          std::complex<float> alpha = 1., std::complex<float> beta = 0.,
          CBLAS_TRANSPOSE trA = CblasNoTrans, int ldA = -1, int incX = 1,
          int incY = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? KL + KU + 1 : ldA );
        cblas_cgbmv( order, trA, M, N, KL, KU, (const LINALG_CCOMPLEX)&alpha,
                     (const LINALG_CCOMPLEX)A, ldA, (const LINALG_CCOMPLEX)x,
                     incX, (const LINALG_CCOMPLEX)&beta, (LINALG_CCOMPLEX)y,
                     incY );
    }
    // ------------------------------------------------------------------------
    inline void
    gbmv( int M, int N, int KL, int KU, const std::complex<double> *A,
          const std::complex<double> *x, std::complex<double> *y,
          std::complex<double> alpha = 1., std::complex<double> beta = 0.,
          CBLAS_TRANSPOSE trA = CblasNoTrans, int ldA = -1, int incX = 1,
          int incY = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? KL + KU + 1 : ldA );
        cblas_zgbmv( order, trA, M, N, KL, KU, (const LINALG_ZCOMPLEX)&alpha,
                     (const LINALG_ZCOMPLEX)A, ldA, (const LINALG_ZCOMPLEX)x,
                     incX, (const LINALG_ZCOMPLEX)&beta, (LINALG_ZCOMPLEX)y,
                     incY );
    }
    // ===================== Triangular Matrix - vector product ===============
    inline void trmv( int N, const float *A, float *X,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_strmv( order, Uplo, trA, Diag, N, A, ldA, X, incX );
    }
    // ------------------------------------------------------------------------
    inline void trmv( int N, const double *A, double *X,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_dtrmv( order, Uplo, trA, Diag, N, A, ldA, X, incX );
    }
    // ------------------------------------------------------------------------
    inline void trmv( int N, const std::complex<float> *A,
                      std::complex<float> *X, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_ctrmv( order, Uplo, trA, Diag, N, (const LINALG_CCOMPLEX)A, ldA,
                     (LINALG_CCOMPLEX)X, incX );
    }
    // ------------------------------------------------------------------------
    inline void trmv( int N, const std::complex<double> *A,
                      std::complex<double> *X, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_ztrmv( order, Uplo, trA, Diag, N, (const LINALG_ZCOMPLEX)A, ldA,
                     (LINALG_ZCOMPLEX)X, incX );
    }
    // ====================== Triangular banded matrix - vector product =======
    inline void tbmv( int N, int K, const float *A, float *X,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? K + 1 : ldA );
        cblas_stbmv( order, Uplo, trA, Diag, N, K, A, ldA, X, incX );
    }
    // ------------------------------------------------------------------------
    inline void tbmv( int N, int K, const double *A, double *X,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? K + 1 : ldA );
        cblas_dtbmv( order, Uplo, trA, Diag, N, K, A, ldA, X, incX );
    }
    // ------------------------------------------------------------------------
    inline void tbmv( int N, int K, const std::complex<float> *A,
                      std::complex<float> *X, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? K + 1 : ldA );
        cblas_ctbmv( order, Uplo, trA, Diag, N, K, (const LINALG_CCOMPLEX)A,
                     ldA, (LINALG_CCOMPLEX)X, incX );
    }
    // ------------------------------------------------------------------------
    inline void tbmv( int N, int K, const std::complex<double> *A,
                      std::complex<double> *X, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? K + 1 : ldA );
        cblas_ztbmv( order, Uplo, trA, Diag, N, K, (const LINALG_ZCOMPLEX)A,
                     ldA, (LINALG_ZCOMPLEX)X, incX );
    }
    // ================ Triangular Packed Matrix - Vector product =============
    inline void tpmv( int N, const float *A, float *x,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int incX = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        cblas_stpmv( order, Uplo, trA, Diag, N, A, x, incX );
    }
    // ------------------------------------------------------------------------
    inline void tpmv( int N, const double *A, double *x,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int incX = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        cblas_dtpmv( order, Uplo, trA, Diag, N, A, x, incX );
    }
    // ------------------------------------------------------------------------
    inline void tpmv( int N, const std::complex<float> *A,
                      std::complex<float> *x, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int incX = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        cblas_ctpmv( order, Uplo, trA, Diag, N, (const LINALG_CCOMPLEX)A,
                     (LINALG_CCOMPLEX)x, incX );
    }
    // ------------------------------------------------------------------------
    inline void tpmv( int N, const std::complex<double> *A,
                      std::complex<double> *x, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int incX = 1,
                      CBLAS_ORDER order = CblasColMajor ) {
        cblas_ztpmv( order, Uplo, trA, Diag, N, (const LINALG_ZCOMPLEX)A,
                     (LINALG_ZCOMPLEX)x, incX );
    }
    // =============== Solve triangular linear system =========================
    inline void trsv( int N, const float *A, float *X,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_strsv( order, Uplo, trA, Diag, N, A, ldA, X, incX );
    }
    // ------------------------------------------------------------------------
    inline void trsv( int N, const double *A, double *X,
                      CBLAS_UPLO      Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA  = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_dtrsv( order, Uplo, trA, Diag, N, A, ldA, X, incX );
    }
    // ------------------------------------------------------------------------
    inline void trsv( int N, const std::complex<float> *A,
                      std::complex<float> *X, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_ctrsv( order, Uplo, trA, Diag, N, (const LINALG_CCOMPLEX)A, ldA,
                     (LINALG_CCOMPLEX)X, incX );
    }
    // ------------------------------------------------------------------------
    inline void trsv( int N, const std::complex<double> *A,
                      std::complex<double> *X, CBLAS_UPLO Uplo = CblasUpper,
                      CBLAS_TRANSPOSE trA = CblasNoTrans,
                      CBLAS_DIAG Diag = CblasNonUnit, int ldA = -1,
                      int incX = 1, CBLAS_ORDER order = CblasColMajor ) {
        ldA = ( -1 == ldA ? N : ldA );
        cblas_ztrsv( order, Uplo, trA, Diag, N, (const LINALG_ZCOMPLEX)A, ldA,
                     (LINALG_ZCOMPLEX)X, incX );
    }
}
#undef LINALG_CCOMPLEX
#undef LINALG_ZCOMPLEX
#undef LINALG_CRET
#undef LINALG_ZRET
#endif
