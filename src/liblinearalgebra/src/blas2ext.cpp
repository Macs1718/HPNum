# include "linearalgebra/blas2ext"
namespace LinearAlgebra
{
    template void sparse_gembv<float>( const std::size_t& nbRows, const std::size_t& nbCols,
                                       const float* coefs, const float& alpha,
                                       const std::size_t* ind_u, const float* u,
                                       const float& beta, const std::size_t* ind_v, float *v);

    template void sparse_gembv<double>( const std::size_t& nbRows, const std::size_t& nbCols,
                                        const double* coefs, const double& alpha,
                                        const std::size_t* ind_u, const double* u,
                                        const double& beta, const std::size_t* ind_v, double *v);

    template void sparse_gembv<std::complex<float>>( const std::size_t& nbRows, const std::size_t& nbCols,
                                                     const std::complex<float>* coefs, const std::complex<float>& alpha,
                                                     const std::size_t* ind_u, const std::complex<float>* u,
                                                     const std::complex<float>& beta, const std::size_t* ind_v, std::complex<float> *v);

    template void sparse_gembv<std::complex<double>>( const std::size_t& nbRows, const std::size_t& nbCols,
                                                      const std::complex<double>* coefs, const std::complex<double>& alpha,
                                                      const std::size_t* ind_u, const std::complex<double>* u,
                                                      const std::complex<double>& beta, const std::size_t* ind_v, std::complex<double> *v);

}
