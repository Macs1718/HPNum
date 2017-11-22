# include <vector>
# include "linearalgebra/blas2.hpp"
namespace LinearAlgebra
{
    template<typename K> void sparse_gembv( const std::size_t& nbRows, const std::size_t& nbCols,
                                            const K* coefs,
                                            const K &alpha,
                                            const std::size_t* ind_u, const K *u,
                                            const K &beta,
                                            const std::size_t* ind_v, K *v)
    {
         std::size_t i;
         std::vector<K> ut(nbCols);
         std::vector<K> vt(nbRows);
         for (i = 0; i < nbCols; i++)
            ut[i] = u[ind_u[i]];
         Blas::gemv( nbRows, nbCols, coefs, ut.data(), vt.data(), alpha);
         for (i = 0; i < nbRows; i++)
            v[ind_v[i]] = beta * v[ind_v[i]] + vt[i];
    }
}
