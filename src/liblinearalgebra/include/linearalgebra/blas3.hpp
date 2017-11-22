// ========================================================================
// ==           Blas encapsulation for template compatibility            ==
// ========================================================================
#ifndef _LINEARALGEBRA_BLAS3_HPP_
# define _LINEARALGEBRA_BLAS3_HPP_
# include <complex>

# if defined(USE_MKL)
extern "C" {
#  include <mkl_cblas.h>
}
#  define LINALG_CCOMPLEX void*
#  define LINALG_ZCOMPLEX void*
#  define LINALG_CRET     void*
#  define LINALG_ZRET     void*
# elif defined(USE_OPENBLAS)
# include <cblas.h>
# define LINALG_CCOMPLEX float*
# define LINALG_ZCOMPLEX double*
# define LINALG_CRET     openblas_complex_float*
# define LINALG_ZRET     openblas_complex_double*
# else
extern "C" {
#  include <cblas.h>
}
#  define LINALG_CCOMPLEX void*
#  define LINALG_ZCOMPLEX void*
#  define LINALG_CRET     void*
#  define LINALG_ZRET     void*
# endif

namespace Blas
{
// ============= General matrix-matrix product ============================
    inline void
    gemm(int M, int N, int K, const float* A, const float* B, float* C,
	 CBLAS_TRANSPOSE trA, CBLAS_TRANSPOSE trB,
	 float alpha = 1., float beta = 0., int ldA = -1, int ldB = -1, int ldC = -1,
	 CBLAS_ORDER order = CblasColMajor) {
	if (CblasNoTrans == trA)
	    ldA = (-1 == ldA ? (CblasColMajor == order ? M : K) : ldA);
	else
	    ldA = (-1 == ldA ? (CblasColMajor == order ? K : M) : ldA);
	if (CblasNoTrans == trB)
	    ldB = (-1 == ldB ? (CblasColMajor == order ? K : N) : ldB);
	else
	    ldB = (-1 == ldB ? (CblasColMajor == order ? N : K) : ldB);
	ldC = (-1 == ldC ? (CblasColMajor == order ? M : N) : ldC);
	cblas_sgemm(order, trA, trB, M, N, K, alpha, A, ldA, B, ldB, beta, C, ldC);
    }
// ------------------------------------------------------------------------
    inline void
    gemm(int M, int N, int K, const double* A, const double* B, double* C,
	 CBLAS_TRANSPOSE trA, CBLAS_TRANSPOSE trB,
	 double alpha = 1., double beta = 0., int ldA = -1, int ldB = -1, int ldC = -1,
	 CBLAS_ORDER order = CblasColMajor) {
	if (CblasNoTrans == trA)
	    ldA = (-1 == ldA ? (CblasColMajor == order ? M : K) : ldA);
	else
	    ldA = (-1 == ldA ? (CblasColMajor == order ? K : M) : ldA);
	if (CblasNoTrans == trB)
	    ldB = (-1 == ldB ? (CblasColMajor == order ? K : N) : ldB);
	else
	    ldB = (-1 == ldB ? (CblasColMajor == order ? N : K) : ldB);
	ldC = (-1 == ldC ? (CblasColMajor == order ? M : N) : ldC);
	cblas_dgemm(order, trA, trB, M, N, K, alpha, A, ldA, B, ldB, beta, C, ldC);
    }
// ------------------------------------------------------------------------
    inline void
    gemm(int M, int N, int K, const std::complex<float>* A, const std::complex<float>* B,
	 std::complex<float>* C, CBLAS_TRANSPOSE trA, CBLAS_TRANSPOSE trB,
	 std::complex<float> alpha = 1., std::complex<float> beta = 0.,
	 int ldA = -1, int ldB = -1, int ldC = -1, CBLAS_ORDER order = CblasColMajor) {
	if (CblasNoTrans == trA)
	    ldA = (-1 == ldA ? (CblasColMajor == order ? M : K) : ldA);
	else
	    ldA = (-1 == ldA ? (CblasColMajor == order ? K : M) : ldA);
	if (CblasNoTrans == trB)
	    ldB = (-1 == ldB ? (CblasColMajor == order ? K : N) : ldB);
	else
	    ldB = (-1 == ldB ? (CblasColMajor == order ? N : K) : ldB);
	ldC = (-1 == ldC ? (CblasColMajor == order ? M : N) : ldC);
	cblas_cgemm(order, trA, trB, M, N, K, (const LINALG_CCOMPLEX)&alpha,
		    (const LINALG_CCOMPLEX)A, ldA,
		    (const LINALG_CCOMPLEX)B, ldB, (const LINALG_CCOMPLEX)&beta,
		    (LINALG_CCOMPLEX)C, ldC);
    }
// ------------------------------------------------------------------------
    inline void
    gemm(int M, int N, int K, const std::complex<double>* A, const std::complex<double>* B,
	 std::complex<double>* C, CBLAS_TRANSPOSE trA, CBLAS_TRANSPOSE trB,
	 std::complex<double> alpha = 1., std::complex<double> beta = 0.,
	 int ldA = -1, int ldB = -1, int ldC = -1, CBLAS_ORDER order = CblasColMajor) {
	if (CblasNoTrans == trA)
	    ldA = (-1 == ldA ? (CblasColMajor == order ? M : K) : ldA);
	else
	    ldA = (-1 == ldA ? (CblasColMajor == order ? K : M) : ldA);
	if (CblasNoTrans == trB)
	    ldB = (-1 == ldB ? (CblasColMajor == order ? K : N) : ldB);
	else
	    ldB = (-1 == ldB ? (CblasColMajor == order ? N : K) : ldB);
	ldC = (-1 == ldC ? (CblasColMajor == order ? M : N) : ldC);
	cblas_zgemm(order, trA, trB, M, N, K, (const LINALG_ZCOMPLEX)&alpha,
		    (const LINALG_ZCOMPLEX)A, ldA,
		    (const LINALG_ZCOMPLEX)B, ldB, (const LINALG_ZCOMPLEX)&beta,
		    (LINALG_ZCOMPLEX)C, ldC);
    }
// ========================================================================
}
# undef LINALG_CCOMPLEX
# undef LINALG_ZCOMPLEX
# undef LINALG_CRET
# undef LINALG_ZRET
#endif
