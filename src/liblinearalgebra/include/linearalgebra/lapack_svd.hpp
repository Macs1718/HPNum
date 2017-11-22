// ========================================================================
// ==      C++ encapsulation for lapack routines solving SVD problems    ==
// ========================================================================
#ifndef _LINEARALGEBRA_BLASLAPACK_LAPACKSVD_HPP_
# define _LINEARALGEBRA_BLASLAPACK_LAPACKSVD_HPP_
# include <algorithm>
# include <complex>
# if defined(USEMKL)
#   include <mkl_lapacke.h>
# else
#   include <lapacke.h>
# endif

namespace Lapack
{

    inline void gesvd(char jobu, char jobv, int M, int N, float *A, float *S, float *U, float *V,
		      int ldA = -1, int ldU = -1, int ldV = -1)
    {
	int minDim = std::min(M, N);
	ldA = (-1 == ldA ? M : ldA);
	ldU = (-1 == ldU ? ( jobu == 'S' || jobu == 'A' ? M : 1) : ldU);
	ldV = (-1 == ldV ? ( jobv == 'A' ? N : (jobv == 'S' ? minDim : 1)) : ldV);
	float *work = new float[minDim];
	LAPACKE_sgesvd(LAPACK_COL_MAJOR, jobu, jobv, M, N, A, ldA, S, U, ldU,
		       V, ldV, work);
	delete [] work;
    }

    inline void gesvd(char jobu, char jobv, int M, int N, double *A, double *S, double *U, double *V,
		      int ldA = -1, int ldU = -1, int ldV = -1)
    {
	int minDim = std::min(M, N);
	ldA = (-1 == ldA ? M : ldA);
	ldU = (-1 == ldU ? ( jobu == 'S' || jobu == 'A' ? M : 1) : ldU);
	ldV = (-1 == ldV ? ( jobv == 'A' ? N : (jobv == 'S' ? minDim : 1)) : ldV);
	double *work = new double[minDim];
	LAPACKE_dgesvd(LAPACK_COL_MAJOR, jobu, jobv, M, N, A, ldA, S, U, ldU,
		       V, ldV, work);
	delete [] work;
    }

    inline void gesvd(char jobu, char jobv, int M, int N, std::complex<float> *A, float *S, std::complex<float> *U, std::complex<float> *V,
		      int ldA = -1, int ldU = -1, int ldV = -1)
    {
	int minDim = std::min(M, N);
	ldA = (-1 == ldA ? M : ldA);
	ldU = (-1 == ldU ? ( jobu == 'S' || jobu == 'A' ? M : 1) : ldU);
	ldV = (-1 == ldV ? ( jobv == 'A' ? N : (jobv == 'S' ? minDim : 1)) : ldV);
	float *work = new float[minDim];
	LAPACKE_cgesvd(LAPACK_COL_MAJOR, jobu, jobv, M, N,
		       (lapack_complex_float *) A, ldA, S,
		       (lapack_complex_float *) U, ldU,
		       (lapack_complex_float *) V, ldV, work);
	delete [] work;
    }

    inline void gesvd(char jobu, char jobv, int M, int N, std::complex<double> *A, double *S,
		      std::complex<double> *U, std::complex<double> *V, int ldA = -1, int ldU = -1, int ldV = -1)
    {
	int minDim = std::min(M, N);
	ldA = (-1 == ldA ? M : ldA);
	ldU = (-1 == ldU ? ( jobu == 'S' || jobu == 'A' ? M : 1) : ldU);
	ldV = (-1 == ldV ? ( jobv == 'A' ? N : (jobv == 'S' ? minDim : 1)) : ldV);
	double *work = new double[minDim];
	LAPACKE_zgesvd(LAPACK_COL_MAJOR, jobu, jobv, M, N,
		       (lapack_complex_double *) A, ldA, S,
		       (lapack_complex_double *) U, ldU,
		       (lapack_complex_double *) V, ldV, work);
	delete [] work;
    }

}
//# undef LAPACK
//# undef TRANSP
#endif
