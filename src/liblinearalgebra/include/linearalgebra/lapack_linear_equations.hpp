// ========================================================================
// ==  C++ encapsultation for Lapack routines solving linear equations   ==
// ========================================================================
#ifndef _LINEARALGEBRA_LAPACK_LINEAR_EQUATIONS_HPP_
#define _LINEARALGEBRA_LAPACK_LINEAR_EQUATIONS_HPP_
#include <complex>
#include <stdexcept>
#if defined( USE_MKL )
#include <mkl_lapacke.h>
#else
#include <lapacke.h>
#endif

namespace Lapack {
    // LU factorization of a general m-by-n matrix
    // ============ LU factorization of a general m-by-n matrix ===============

    /**
     * @brief      compute an LU factorization of a general M-by-N
     *             matrix A using partial pivoting with row interchanges
     *
     * @param[in]  M     The number of rows of the matrix A.  M >= 0.
     * @param[in]  N     The number of columns of the matrix A.  N >= 0.
     * @param      A     On entry, the M-by-N matrix to be factored.  On
     *                   exit, the factors L and U from the factorization A =
     *                   P*L*U; the unit diagonal elements of L are not
     *                   stored.
     * @param      ipiv  The pivot indices. Row i of the matrix was interchanged
     *                   with row ipiv[i]
     * @param[in]  lda   The leading dimension of the array A. lda >= max(1,M)
     */
    inline void getrf( int M, int N, float *A, lapack_int *ipiv,
                       int lda = -1 ) {
        lapack_int info;
        lda  = ( -1 == lda ? M : lda );
        info = LAPACKE_sgetrf( LAPACK_COL_MAJOR, M, N, A, lda, ipiv );

        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "Invalid %dth arg. for lapack getrf", -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg,
                         "Diagonal block is singular :\n\t %d th pivot is null",
                         info );
                throw std::runtime_error( msg );
            }
        }
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      compute an LU factorization of a general M-by-N
     *             matrix A using partial pivoting with row interchanges
     *
     * @param[in]  M     The number of rows of the matrix A.  M >= 0.
     * @param[in]  N     The number of columns of the matrix A.  N >= 0.
     * @param      A     On entry, the M-by-N matrix to be factored.  On
     *                   exit, the factors L and U from the factorization A =
     *                   P*L*U; the unit diagonal elements of L are not
     *                   stored.
     * @param      ipiv  The pivot indices. Row i of the matrix was interchanged
     *                   with row ipiv[i]
     * @param[in]  lda   The leading dimension of the array A. lda >= max(1,M)
     */ inline void getrf( int M, int N, double *A, lapack_int *ipiv,
                           int lda = -1 ) {
        lapack_int info;
        lda  = ( -1 == lda ? M : lda );
        info = LAPACKE_dgetrf( LAPACK_COL_MAJOR, M, N, A, lda, ipiv );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "Invalid %dth arg. for lapack getrf", -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "Diagonal block is singular :\n"
                              "\t %d th pivot is null",
                         info );
                throw std::runtime_error( msg );
            }
        }
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      compute an LU factorization of a general M-by-N
     *             matrix A using partial pivoting with row interchanges
     *
     * @param[in]  M     The number of rows of the matrix A.  M >= 0.
     * @param[in]  N     The number of columns of the matrix A.  N >= 0.
     * @param      A     On entry, the M-by-N matrix to be factored.  On
     *                   exit, the factors L and U from the factorization A =
     *                   P*L*U; the unit diagonal elements of L are not
     *                   stored.
     * @param      ipiv  The pivot indices. Row i of the matrix was interchanged
     *                   with row ipiv[i]
     * @param[in]  lda   The leading dimension of the array A. lda >= max(1,M)
     */
    inline void getrf( int M, int N, std::complex<float> *A, lapack_int *ipiv,
                       int lda = -1 ) {
        lapack_int info;
        lda  = ( -1 == lda ? M : lda );
        info = LAPACKE_cgetrf( LAPACK_COL_MAJOR, M, N,
                               (lapack_complex_float *)A, lda, ipiv );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "Invalid %dth arg. for lapack getrf", -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "Diagonal block is singular :\n"
                              "\t %d th pivot is null",
                         info );
                throw std::runtime_error( msg );
            }
        }
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      compute an LU factorization of a general M-by-N
     *             matrix A using partial pivoting with row interchanges
     *
     * @param[in]  M     The number of rows of the matrix A.  M >= 0.
     * @param[in]  N     The number of columns of the matrix A.  N >= 0.
     * @param      A     On entry, the M-by-N matrix to be factored.  On
     *                   exit, the factors L and U from the factorization A =
     *                   P*L*U; the unit diagonal elements of L are not
     *                   stored.
     * @param      ipiv  The pivot indices. Row i of the matrix was interchanged
     *                   with row ipiv[i]
     * @param[in]  lda   The leading dimension of the array A. lda >= max(1,M)
     */
    inline void getrf( int M, int N, std::complex<double> *A, lapack_int *ipiv,
                       int lda = -1 ) {
        lapack_int info;
        lda  = ( -1 == lda ? M : lda );
        info = LAPACKE_zgetrf( LAPACK_COL_MAJOR, M, N,
                               (lapack_complex_double *)A, lda, ipiv );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "Invalid %dth arg. for lapack getrf", -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "Diagonal block is singular :\n"
                              "\t %d th pivot is null",
                         info );
                throw std::runtime_error( msg );
            }
        }
    }
    // ========================================================================
    /**
     * @brief      solve a system of linear equations  A * X = B or A'* X = B
     *             with a general N-by-N matrix A using the LU factorization
     *             computed by getrf.
     *
     * @param[in]  N     The order of the matrix. N >= 0
     * @param[in]  NRHS  The number of right hand sides. NRHS >= 0.
     * @param[in]  A     The factors L and U from the factorization A = P*L*U
     *                   as computed by getrf.
     * @param[in]  ipiv  The pivot indices from getrf.
     * @param      B     On entry, the right hand side matrix B. On exit, the
     *                   solution matrix X.
     * @param[in]  trA   Specify the form of the linear equations ( transpose or
     *                   not )
     * @param[in]  ldA   The leading dimension of A
     * @param[in]  ldB   The leading dimension of B
     */
    inline void getrs( int N, int NRHS, const float *A, const lapack_int *ipiv,
                       float *B, char trA = 'N', int ldA = -1, int ldB = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? N : ldA );
        ldB  = ( -1 == ldB ? N : ldB );
        info = LAPACKE_sgetrs( LAPACK_COL_MAJOR, trA, N, NRHS, A, ldA, ipiv, B,
                               ldB );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "getrs : %d th parameter has illegal value !",
                         -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "getrs : undocumented error ?" );
                throw std::runtime_error( msg );
            }
        }
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      solve a system of linear equations  A * X = B or A'* X = B
     *             with a general N-by-N matrix A using the LU factorization
     *             computed by getrf.
     *
     * @param[in]  N     The order of the matrix. N >= 0
     * @param[in]  NRHS  The number of right hand sides. NRHS >= 0.
     * @param[in]  A     The factors L and U from the factorization A = P*L*U
     *                   as computed by getrf.
     * @param[in]  ipiv  The pivot indices from getrf.
     * @param      B     On entry, the right hand side matrix B. On exit, the
     *                   solution matrix X.
     * @param[in]  trA   Specify the form of the linear equations ( transpose or
     *                   not )
     * @param[in]  ldA   The leading dimension of A
     * @param[in]  ldB   The leading dimension of B
     */
    inline void getrs( int N, int NRHS, const double *A, const lapack_int *ipiv,
                       double *B, char trA = 'N', int ldA = -1, int ldB = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? N : ldA );
        ldB  = ( -1 == ldB ? N : ldB );
        info = LAPACKE_dgetrs( LAPACK_COL_MAJOR, trA, N, NRHS, A, ldA, ipiv, B,
                               ldB );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "getrs : %d th parameter has illegal value !",
                         -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "getrs : undocumented error ?" );
                throw std::runtime_error( msg );
            }
        }
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      solve a system of linear equations  A * X = B or A'* X = B
     *             with a general N-by-N matrix A using the LU factorization
     *             computed by getrf.
     *
     * @param[in]  N     The order of the matrix. N >= 0
     * @param[in]  NRHS  The number of right hand sides. NRHS >= 0.
     * @param[in]  A     The factors L and U from the factorization A = P*L*U
     *                   as computed by getrf.
     * @param[in]  ipiv  The pivot indices from getrf.
     * @param      B     On entry, the right hand side matrix B. On exit, the
     *                   solution matrix X.
     * @param[in]  trA   Specify the form of the linear equations ( transpose or
     *                   not )
     * @param[in]  ldA   The leading dimension of A
     * @param[in]  ldB   The leading dimension of B
     */
    inline void getrs( int N, int NRHS, const std::complex<float> *A,
                       const lapack_int *ipiv, std::complex<float> *B,
                       char trA = 'N', int ldA = -1, int ldB = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? N : ldA );
        ldB  = ( -1 == ldB ? N : ldB );
        info = LAPACKE_cgetrs( LAPACK_COL_MAJOR, trA, N, NRHS,
                               (lapack_complex_float *)A, ldA, ipiv,
                               (lapack_complex_float *)B, ldB );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "getrs : %d th parameter has illegal value !",
                         -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "getrs : undocumented error ?" );
                throw std::runtime_error( msg );
            }
        }
    }
    // ------------------------------------------------------------------------
    /**
     * @brief      solve a system of linear equations  A * X = B or A'* X = B
     *             with a general N-by-N matrix A using the LU factorization
     *             computed by getrf.
     *
     * @param[in]  N     The order of the matrix. N >= 0
     * @param[in]  NRHS  The number of right hand sides. NRHS >= 0.
     * @param[in]  A     The factors L and U from the factorization A = P*L*U
     *                   as computed by getrf.
     * @param[in]  ipiv  The pivot indices from getrf.
     * @param      B     On entry, the right hand side matrix B. On exit, the
     *                   solution matrix X.
     * @param[in]  trA   Specify the form of the linear equations ( transpose or
     *                   not )
     * @param[in]  ldA   The leading dimension of A
     * @param[in]  ldB   The leading dimension of B
     */
    inline void getrs( int N, int NRHS, const std::complex<double> *A,
                       const lapack_int *ipiv, std::complex<double> *B,
                       char trA = 'N', int ldA = -1, int ldB = -1 ) {
        lapack_int info;
        ldA  = ( -1 == ldA ? N : ldA );
        ldB  = ( -1 == ldB ? N : ldB );
        info = LAPACKE_zgetrs( LAPACK_COL_MAJOR, trA, N, NRHS,
                               (lapack_complex_double *)A, ldA, ipiv,
                               (lapack_complex_double *)B, ldB );
        if ( info != 0 ) {
            char msg[255];
            if ( info < 0 ) {
                sprintf( msg, "getrs : %d th parameter has illegal value !",
                         -info );
                throw std::invalid_argument( msg );
            } else {
                sprintf( msg, "getrs : undocumented error ?" );
                throw std::runtime_error( msg );
            }
        }
    }
    // ========================================================================
}

//# undef LAPACK
#endif
