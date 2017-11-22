#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "core/logger.hpp"
#include "core/multitimer.hpp"
#include "core/std_cpp_chronometer.hpp"

/**
 * @brief      Représentation d'un bloc de matrice
 *
 *    Un bloc de matrice est une (sous)-matrice ayant pour paramêtres :
 *         - La dimension du bloc ( nombre de lignes x nombre de colonnes )
 *         - Les éléments du bloc matriciel
 *    Le premier indice du bloc n'est pas décrit dans cette classe, et sera donc si
 *    besoin est, à la charge de l'utilisateur.
 *
 * @tparam     K     Le type des éléments contenus dans le bloc matriciel
 */
template < typename K >
class BlockMatrix : public std::vector< K > {
public:
    /**
     * @brief      Construit un bloc de dimension nul par défaut
     */
    BlockMatrix( ) = default;
    /**
     * @brief      Construit un bloc de dimension nrows x ncols
     *
     * @param[in]  nrows  Le nombre de lignes du bloc matriciel
     * @param[in]  ncols  Le nombre de colonnes du bloc matriciel.
     */
    BlockMatrix( std::size_t nrows, std::size_t ncols )
        : std::vector< K >( nrows * ncols ), m_nrows( nrows ), m_ncols( ncols ) {}
    /**
     * @brief      Construit un bloc matriciel de dimension nrows x ncols avec la valeur val pour chaque élément
     *
     * @param[in]  nrows  The nrows
     * @param[in]  ncols  The ncols
     * @param[in]  val    The value which fills the matrix.
     */
    BlockMatrix( std::size_t nrows, std::size_t ncols, const K &val )
        : std::vector< K >( nrows * ncols, val ), m_nrows( nrows ), m_ncols( ncols ) {}
    BlockMatrix( const BlockMatrix &A ) = default;
    BlockMatrix( BlockMatrix &&A )      = default;
    ~BlockMatrix( )                     = default;

    BlockMatrix &operator=( const BlockMatrix &A ) = default;
    BlockMatrix &operator=( BlockMatrix &&A ) = default;

    std::size_t getNRows( ) const {
        return m_nrows;
    }
    std::size_t getNCols( ) const {
        return m_ncols;
    }

    const K &operator( )( std::size_t i, std::size_t j ) const && {
        assert( i < m_nrows );
        assert( j < m_ncols );
        return ( *this )[i + j * m_nrows];
    }
    K &operator( )( std::size_t i, std::size_t j ) & {
        assert( i < m_nrows );
        assert( j < m_ncols );
        return ( *this )[i + j * m_nrows];
    }
    const K &operator( )( std::size_t i, std::size_t j ) const & {
        assert( i < m_nrows );
        assert( j < m_ncols );
        return ( *this )[i + j * m_nrows];
    }

private:
    std::size_t m_nrows, m_ncols;
};

/**
 * brief       Compute tensorial vectors
 *
 * Compute two pairs of vectors. Each pair of vectors define a tensor product to
 * compute respectivly the coefficients of the A and B matrices.
 */
template < typename K >
auto computeTensorVectors( std::size_t dim ) {
    const K          pi = std::acos( K( -1 ) );
    std::vector< K > u1_r( dim ), v1_c( dim ), u2_r( dim ), v2_c( dim );
    const K          lx = 1., ly = 2.;
    for ( size_t i = 0; i < dim; ++i ) {
        u1_r[i] = std::cos( 2 * i * pi / dim );
        v1_c[i] = std::sin( 2 * i * pi / dim );
        u2_r[i] = i * lx / dim;
        v2_c[i] = i * ly / dim;
    }
    return std::make_tuple( u1_r, v1_c, u2_r, v2_c );
}
// -----------------------------------------------------------------------------
/**
 * brief       Compute a matrix from a pair of vector
 *
 * Compute a matrix block with the tensor product of a pair of vectors.
 */
template < typename K >
auto computeMatrice( const std::vector< K > &u_row, const std::vector< K > &v_col ) {
    BlockMatrix< K > A( u_row.size( ), v_col.size( ) );
    for ( std::size_t irow = 0; irow < u_row.size( ); ++irow ) {
        for ( std::size_t jcol = 0; jcol < v_col.size( ); ++jcol ) { A( irow, jcol ) = u_row[irow] * v_col[jcol]; }
    }
    return A;
}
// -----------------------------------------------------------------------------
/**
 * brief       Compute product of block matrix with block matrix
 *
 * Product of two block matrices A  and B  which is added to a third block matrix C
 */
template < typename K >
void prodMatrixMatrixBloc( const BlockMatrix< K > &A, const BlockMatrix< K > &B, BlockMatrix< K > &C ) {
    assert( A.getNCols( ) == B.getNRows( ) );
    for ( std::size_t k = 0; k < A.getNCols( ); ++k ) {
        for ( std::size_t j = 0; j < B.getNCols( ); ++j ) {
            for ( std::size_t i = 0; i < A.getNRows( ); ++i ) 
                { C( i, j ) += A( i, k ) * B( k, j ); }
        }
    }
}
// -----------------------------------------------------------------------------
/**
 * brief       Dot product of two vectors
 *
 * Dot product of two local vectors ( not a global dot product ! )
 */
template < typename K >
K dotProduct( const std::vector< K > &u, const std::vector< K > &v ) {
    assert( u.size( ) == v.size( ) );
    K sum = 0;
    for ( std::size_t i = 0; i < u.size( ); ++i ) sum += u[i] * v[i];
    return sum;
}
// -----------------------------------------------------------------------------
/**
 * brief       Verification of the result of the matrix-matrix product
 *
 * Verification of the product matrix matrix result. The result matrix must
 * verify the following formulae :
 *  A = uA x vA^{T} and B = uB x vB^{T}
 *  so C = A.B = (vA|uB) uA x vB^{T}
 *  a.k.a C_ij = (vA|uB) uA_{i}vB_{j}
 */
template < typename K >
bool verifyProdMatMat( std::size_t dimBlock, const K &alpha, const std::vector< K > &uA, const std::vector< K > &vB,
                       const BlockMatrix< K > &C ) {
    bool isOK = true;
    for ( std::size_t i = 0; i < dimBlock; ++i ) {
        for ( std::size_t j = 0; j < dimBlock; ++j ) {
            K val = alpha * uA[i] * vB[j];
            if ( std::abs( val - C( i, j ) ) > 1.E-6 * std::abs( C( i, j ) ) ) {
                std::cerr << "Detecting a problem in result coefficients : "
                          << "Value computed : " << C( i, j ) << " and value excepted : " << val << std::endl;
                isOK = false;
                break;
            }
        }
    }
    return isOK;
}
// _____________________________________________________________________________
// =============================================================================
int main( int nargs, char *argv[] ) {
    bool profiled = false;
    bool trace = false;
    for ( int iargs = 2; iargs < nargs; ++iargs ) {
        if ( std::string( argv[iargs] ) == std::string( "trace" ) )
            trace = true;

        if ( std::string( argv[iargs] ) == std::string( "profile" ) ) { profiled = true; }
    }

    Core::MultiTimer< std::string > timer;

    std::size_t dim      = 120;
    if ( nargs > 1 ) dim = std::stoul( std::string( argv[1] ) );
    timer.subscribe< Core::StdChronometer >( "Compute tensor vectors" );
    timer.subscribe< Core::StdChronometer >( "Compute matrices" );
    timer.subscribe< Core::StdChronometer >( "Product Matrix-matrix" );
    timer.subscribe< Core::StdChronometer >( "Verify Matrix-matrix" );

    timer["Compute tensor vectors"].start( );
    std::vector< double > uA, vA, uB, vB;
    std::tie( uA, vA, uB, vB ) = computeTensorVectors< double >( dim );
    timer["Compute tensor vectors"].stop( );

    timer["Compute matrices"].start( );
    auto A = computeMatrice( uA, vA );
    auto B = computeMatrice( uB, vB );
    timer["Compute matrices"].stop( );
    BlockMatrix< double > C( A.getNRows( ), B.getNCols( ) );
    // Parallel product :

    timer["Product Matrix-matrix"].start( );
    prodMatrixMatrixBloc( A, B, C );
    timer["Product Matrix-matrix"].stop( );

    timer["Verify Matrix-matrix"].start( );
    std::tie( std::ignore, vA, uB, std::ignore ) = computeTensorVectors< double >( dim );
    double vAdotuB = dotProduct( vA, uB );
    if ( verifyProdMatMat( dim, vAdotuB, uA, vB, C ) ) {
        std::cout << Core::Logger::BGreen << "Test passed." << Core::Logger::Normal << std::endl;
    } else {
    }
    timer["Verify Matrix-matrix"].stop( );
    std::cout << timer << std::endl;
    return EXIT_SUCCESS;
}
