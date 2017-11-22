#ifndef _LINEARALGEBRA_BLAS2EXT_HPP_
# define _LINEARALGEBRA_BLAS2EXT_HPP_
# include <cstddef>

namespace LinearAlgebra
{
    template<typename K> void sparse_gembv( const std::size_t& nbRows, const std::size_t& nbCols,
                                            const K* coefs,
                                            const K &alpha,
                                            const std::size_t* ind_u, const K *u,
                                            const K &beta,
                                            const std::size_t* ind_v, K *v);
}

#endif
