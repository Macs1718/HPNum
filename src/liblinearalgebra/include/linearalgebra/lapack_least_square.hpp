// ========================================================================
// == C++ encapsulation for lapack routines solving least square problems==
// ========================================================================
#ifndef _LINEARALGEBRA_BLASLAPACK_LAPACKLEASTSQUARE_HPP_
#define _LINEARALGEBRA_BLASLAPACK_LAPACKLEASTSQUARE_HPP_
#include <complex>
#include <stdexcept>

#include "linearalgebra/blas1.hpp"
#if defined( USE_MKL )
#include <mkl_lapacke.h>
#else
#include <lapacke.h>
#endif
#include <cassert>
#include <cstdlib>

namespace Lapack {
    // QR factorization
    // ========================================================================

    /**
     * @brief      compute a QR factorization of a real M-by-N matrix A
     *
     * @param[in]  M      The number of rows of the matrix A.  M >= 0.
     * @param[in]  N      The number of columns of the matrix A.  N >= 0.
     * @param      A      On entry, the M-by-N matrix A.  On exit, the elements
     *                    on and above the diagonal of the array contain the
     *                    min(M,N)-by-N upper
     *                    trapezoidal matrix R (R is upper triangular if m >=n);
     *                    the elements
     *                    below the diagonal, with the array TAU, represent the
     *                    orthogonal matrix Q
     *                    as a product of min(m,n) elementary reflectors
     *
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  order  Tell if the storage of A is column or row major
     *                    storage.
     *
     * @return     Return the success or an error code for the operation
     */
    inline int geqrf( int M, int N, float *A, float *tau, int ldA = -1,
                      int order = LAPACK_COL_MAJOR ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? M : ldA );
        info = LAPACKE_sgeqrf( order, M, N, A, ldA, tau );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      compute a QR factorization of a real M-by-N matrix A
     *
     * @param[in]  M      The number of rows of the matrix A.  M >= 0.
     * @param[in]  N      The number of columns of the matrix A.  N >= 0.
     * @param      A      On entry, the M-by-N matrix A.  On exit, the elements
     *                    on and above the diagonal of the array contain the
     *                    min(M,N)-by-N upper
     *                    trapezoidal matrix R (R is upper triangular if m >=n);
     *                    the elements
     *                    below the diagonal, with the array TAU, represent the
     *                    orthogonal matrix Q
     *                    as a product of min(m,n) elementary reflectors
     *
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  order  Tell if the storage of A is column or row major
     *                    storage.
     *
     * @return     Return the success or an error code for the operation
     */
    inline int geqrf( int M, int N, double *A, double *tau, int ldA = -1,
                      int order = LAPACK_COL_MAJOR ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? M : ldA );
        info = LAPACKE_dgeqrf( order, M, N, A, ldA, tau );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      compute a QR factorization of a real M-by-N matrix A
     *
     * @param[in]  M      The number of rows of the matrix A.  M >= 0.
     * @param[in]  N      The number of columns of the matrix A.  N >= 0.
     * @param      A      On entry, the M-by-N matrix A.  On exit, the elements
     *                    on and above the diagonal of the array contain the
     *                    min(M,N)-by-N upper
     *                    trapezoidal matrix R (R is upper triangular if m >=n);
     *                    the elements
     *                    below the diagonal, with the array TAU, represent the
     *                    orthogonal matrix Q
     *                    as a product of min(m,n) elementary reflectors
     *
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  order  Tell if the storage of A is column or row major
     *                    storage.
     *
     * @return     Return the success or an error code for the operation
     */
    inline int geqrf( int M, int N, std::complex<float> *A,
                      std::complex<float> *tau, int ldA = -1,
                      int order = LAPACK_COL_MAJOR ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? M : ldA );
        info = LAPACKE_cgeqrf( order, M, N, (lapack_complex_float *)A, ldA,
                               (lapack_complex_float *)tau );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      compute a QR factorization of a real M-by-N matrix A
     *
     * @param[in]  M      The number of rows of the matrix A.  M >= 0.
     * @param[in]  N      The number of columns of the matrix A.  N >= 0.
     * @param      A      On entry, the M-by-N matrix A.  On exit, the elements
     *                    on and above the diagonal of the array contain the
     *                    min(M,N)-by-N upper
     *                    trapezoidal matrix R (R is upper triangular if m >=n);
     *                    the elements
     *                    below the diagonal, with the array TAU, represent the
     *                    orthogonal matrix Q
     *                    as a product of min(m,n) elementary reflectors
     *
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  order  Tell if the storage of A is column or row major
     *                    storage.
     *
     * @return     Return the success or an error code for the operation
     */
    inline int geqrf( int M, int N, std::complex<double> *A,
                      std::complex<double> *tau, int ldA = -1,
                      int order = LAPACK_COL_MAJOR ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? M : ldA );
        info = LAPACKE_zgeqrf( order, M, N, (lapack_complex_double *)A, ldA,
                               (lapack_complex_double *)tau );
        return int( info );
    }
    // ========================================================================

    /**
     * @brief      Overwrite the C matrix with Q^{T}.C where Q is a real
     * orthogonal matrix defined as the product of k elementary reflectors.
     *
     * @param[in]  side   Apply Q on the left or right of C
     * @param[in]  trans  The transposition for Q
     * @param[in]  M      The number of rows of the matrix C. M>= 0
     * @param[in]  N      The number of columns of the matrix C. N >= 0
     * @param[in]  K      The number of elementary reflectors whose product
     * defined the matrix Q.
     * @param[in]  A      The matrix factorized with the geqrf function
     * @param[in]  tau    The scalar factor of the elementary reflector provided
     * by geqrf function
     * @param      C      The MxN matrix on entry, overwritten by T(Q).C or
     * C.T(Q)
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  ldC    The leading dimension of C
     *
     * @return     Return the success or error code for this operation
     */
    inline int mqr( char side, char trans, int M, int N, int K, const float *A,
                    const float *tau, float *C, int ldA = -1, int ldC = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? ( side == 'L' ? M : N ) : ldA );
        ldC  = ( -1 == ldC ? M : ldC );
        info = LAPACKE_sormqr( LAPACK_COL_MAJOR, side, trans, M, N, K, A, ldA,
                               tau, C, ldC );
        return int( info );
    }
    // ........................................................................
    /**
     * @brief      Overwrite the C matrix with Q^{T}.C where Q is a real
     * orthogonal matrix defined as the product of k elementary reflectors.
     *
     * @param[in]  side   Apply Q on the left or right of C
     * @param[in]  trans  The transposition for Q
     * @param[in]  M      The number of rows of the matrix C. M>= 0
     * @param[in]  N      The number of columns of the matrix C. N >= 0
     * @param[in]  K      The number of elementary reflectors whose product
     * defined the matrix Q.
     * @param[in]  A      The matrix factorized with the geqrf function
     * @param[in]  tau    The scalar factor of the elementary reflector provided
     * by geqrf function
     * @param      C      The MxN matrix on entry, overwritten by T(Q).C or
     * C.T(Q)
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  ldC    The leading dimension of C
     *
     * @return     Return the success or error code for this operation
     */
    inline int mqr( char side, char trans, int M, int N, int K, const double *A,
                    const double *tau, double *C, int ldA = -1, int ldC = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? ( side == 'L' ? M : N ) : ldA );
        ldC  = ( -1 == ldC ? M : ldC );
        info = LAPACKE_dormqr( LAPACK_COL_MAJOR, side, trans, M, N, K, A, ldA,
                               tau, C, ldC );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      Overwrite the C matrix with Q^{T}.C where Q is a real
     * orthogonal matrix defined as the product of k elementary reflectors.
     *
     * @param[in]  side   Apply Q on the left or right of C
     * @param[in]  trans  The transposition for Q
     * @param[in]  M      The number of rows of the matrix C. M>= 0
     * @param[in]  N      The number of columns of the matrix C. N >= 0
     * @param[in]  K      The number of elementary reflectors whose product
     * defined the matrix Q.
     * @param[in]  A      The matrix factorized with the geqrf function
     * @param[in]  tau    The scalar factor of the elementary reflector provided
     * by geqrf function
     * @param      C      The MxN matrix on entry, overwritten by T(Q).C or
     * C.T(Q)
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  ldC    The leading dimension of C
     *
     * @return     Return the success or error code for this operation
     */
    inline int mqr( char side, char trans, int M, int N, int K,
                    const std::complex<float> *A,
                    const std::complex<float> *tau, std::complex<float> *C,
                    int ldA = -1, int ldC = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? ( side == 'L' ? M : N ) : ldA );
        ldC  = ( -1 == ldC ? M : ldC );
        info = LAPACKE_cunmqr( LAPACK_COL_MAJOR, side, trans, M, N, K,
                               (const lapack_complex_float *)A, ldA,
                               (const lapack_complex_float *)tau,
                               (lapack_complex_float *)C, ldC );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      Overwrite the C matrix with Q^{T}.C where Q is a real
     * orthogonal matrix defined as the product of k elementary reflectors.
     *
     * @param[in]  side   Apply Q on the left or right of C
     * @param[in]  trans  The transposition for Q
     * @param[in]  M      The number of rows of the matrix C. M>= 0
     * @param[in]  N      The number of columns of the matrix C. N >= 0
     * @param[in]  K      The number of elementary reflectors whose product
     * defined the matrix Q.
     * @param[in]  A      The matrix factorized with the geqrf function
     * @param[in]  tau    The scalar factor of the elementary reflector provided
     * by geqrf function
     * @param      C      The MxN matrix on entry, overwritten by T(Q).C or
     * C.T(Q)
     * @param[in]  ldA    The leading dimension of A
     * @param[in]  ldC    The leading dimension of C
     *
     * @return     Return the success or error code for this operation
     */
    inline int mqr( char side, char trans, int M, int N, int K,
                    const std::complex<double> *A,
                    const std::complex<double> *tau, std::complex<double> *C,
                    int ldA = -1, int ldC = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? ( side == 'L' ? M : N ) : ldA );
        ldC  = ( -1 == ldC ? M : ldC );
        info = LAPACKE_zunmqr( LAPACK_COL_MAJOR, side, trans, M, N, K,
                               (const lapack_complex_double *)A, ldA,
                               (const lapack_complex_double *)tau,
                               (lapack_complex_double *)C, ldC );
        return int( info );
    }
    // ========================================================================

    /**
     * @brief      Compute the QR factorization with column pivoting of a matrix
     * A
     *
     * @param[in]  nRows  Thhe number of rows of the matrix A
     * @param[in]  nCols  The number of columns of the matrix A
     * @param[in]  A      On entry, the matrix A, on exit, the QR factorization
     * @param      jpvt   The column permutation ( on entry changed on exit )
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     *
     * @return     Return the success or error code for this operation
     */
    inline int geqp3( int nRows, int nCols, const float *A, lapack_int *jpvt,
                      float *tau, int ldA = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? nRows : ldA );
        info = LAPACKE_sgeqp3( LAPACK_COL_MAJOR, nRows, nCols, (float *)A, ldA,
                               jpvt, tau );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      Compute the QR factorization with column pivoting of a matrix
     * A
     *
     * @param[in]  nRows  Thhe number of rows of the matrix A
     * @param[in]  nCols  The number of columns of the matrix A
     * @param[in]  A      On entry, the matrix A, on exit, the QR factorization
     * @param      jpvt   The column permutation ( on entry changed on exit )
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     *
     * @return     Return the success or error code for this operation
     */
    inline int geqp3( int nRows, int nCols, const double *A, lapack_int *jpvt,
                      double *tau, int ldA = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? nRows : ldA );
        info = LAPACKE_dgeqp3( LAPACK_COL_MAJOR, nRows, nCols, (double *)A, ldA,
                               jpvt, tau );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      Compute the QR factorization with column pivoting of a matrix
     * A
     *
     * @param[in]  nRows  Thhe number of rows of the matrix A
     * @param[in]  nCols  The number of columns of the matrix A
     * @param[in]  A      On entry, the matrix A, on exit, the QR factorization
     * @param      jpvt   The column permutation ( on entry changed on exit )
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     *
     * @return     Return the success or error code for this operation
     */
    inline int geqp3( int nRows, int nCols, const std::complex<float> *A,
                      lapack_int *jpvt, std::complex<float> *tau,
                      int ldA = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? nRows : ldA );
        info = LAPACKE_cgeqp3( LAPACK_COL_MAJOR, nRows, nCols,
                               (lapack_complex_float *)A, ldA, jpvt,
                               (lapack_complex_float *)tau );
        return int( info );
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      Compute the QR factorization with column pivoting of a matrix
     * A
     *
     * @param[in]  nRows  Thhe number of rows of the matrix A
     * @param[in]  nCols  The number of columns of the matrix A
     * @param[in]  A      On entry, the matrix A, on exit, the QR factorization
     * @param      jpvt   The column permutation ( on entry changed on exit )
     * @param      tau    The scalar factors of the elementary reflectors
     * @param[in]  ldA    The leading dimension of A
     *
     * @return     Return the success or error code for this operation
     */
    inline int geqp3( int nRows, int nCols, const std::complex<double> *A,
                      lapack_int *jpvt, std::complex<double> *tau,
                      int ldA = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? nRows : ldA );
        info = LAPACKE_zgeqp3( LAPACK_COL_MAJOR, nRows, nCols,
                               (lapack_complex_double *)A, ldA, jpvt,
                               (lapack_complex_double *)tau );
        return int( info );
    }
}
#endif
