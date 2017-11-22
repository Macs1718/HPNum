// ========================================================================
// ==                      BoundingBox implementation                    ==
// ========================================================================
namespace {
    template <typename K, int stride = PerComponent>
    void createBox( const std::size_t &nbPoints, const K *X, const K *Y,
                    const K *Z, const std::size_t *indices,
                    ::Geometry::Box<K> &box, K tol ) {
        if ( 0 == nbPoints ) {
            box.reset( );
            return;
        }
        assert( X != NULL );
        assert( Y != NULL );
        assert( Z != NULL );
        auto &minC = box.minCoords( );
        auto &maxC = box.maxCoords( );
        if ( nullptr == indices ) {
            minC[0] = X[0] - tol;
            minC[1] = Y[0] - tol;
            minC[2] = Z[0] - tol;
            maxC[0] = X[0] + tol;
            maxC[1] = Y[0] + tol;
            maxC[2] = Z[0] + tol;
            for ( std::size_t i = 1; i < nbPoints; ++i ) {
                std::size_t ind = i * stride;
                minC[0]         = std::min( minC[0], X[ind] - tol );
                minC[1]         = std::min( minC[1], Y[ind] - tol );
                minC[2]         = std::min( minC[2], Z[ind] - tol );
                maxC[0]         = std::max( maxC[0], X[ind] + tol );
                maxC[1]         = std::max( maxC[1], Y[ind] + tol );
                maxC[2]         = std::max( maxC[2], Z[ind] + tol );
            }
        } else {
            std::size_t ind = indices[0] * stride;
            minC[0]         = X[ind] - tol;
            minC[1]         = Y[ind] - tol;
            minC[2]         = Z[ind] - tol;
            maxC[0]         = X[ind] + tol;
            maxC[1]         = Y[ind] + tol;
            maxC[2]         = Z[ind] + tol;
            for ( std::size_t i = 1; i < nbPoints; ++i ) {
                ind     = indices[i] * stride;
                minC[0] = std::min( minC[0], X[ind] - tol );
                minC[1] = std::min( minC[1], Y[ind] - tol );
                minC[2] = std::min( minC[2], Z[ind] - tol );
                maxC[0] = std::max( maxC[0], X[ind] + tol );
                maxC[1] = std::max( maxC[1], Y[ind] + tol );
                maxC[2] = std::max( maxC[2], Z[ind] + tol );
            }
        }
    }
    // ---------------------------------------------------------------------
    template <typename K, int stride = PerComponent>
    std::size_t sort( const ::Geometry::Box<K> &box, std::size_t nbPoints,
                      const K *X, const K *Y, const K *Z,
                      std::size_t *indices ) {
        std::size_t nbNdInBox = 0; // Counter of number of nodes inside the box
        const auto &minC      = box.minCoords( );
        const auto &maxC      = box.maxCoords( );
        while ( nbNdInBox < nbPoints ) {
            std::size_t ind = indices[nbNdInBox] * stride;
            if ( ( X[ind] > minC[0] ) && ( Y[ind] > minC[1] ) &&
                 ( Z[ind] > minC[2] ) && ( X[ind] < maxC[0] ) &&
                 ( Y[ind] < maxC[1] ) && ( Z[ind] < maxC[2] ) ) {
                nbNdInBox++;
            } else {
                nbPoints--;
                std::swap( indices[nbNdInBox], indices[nbPoints] );
            }
        }
        return nbNdInBox;
    }

} // End anonymous namespace
// ========================================================================
namespace Geometry {
    template <typename K, int stride>
    Box<K> compute_boundingBox( const std::size_t &nbPoints, const K *X,
                                const K *Y, const K *Z,
                                const std::size_t *indices, const K &tol ) {
        Box<K> box;
        createBox<K, stride>( nbPoints, X, Y, Z, indices, box, tol );
        return box;
    }
    // ========================================================================
    template <typename K, int stride>
    std::tuple<std::size_t, Box<K>, std::size_t, Box<K>>
    subdivideAdjustAndSort( const Box<K> &box, const std::size_t &nbPoints,
                            const K *X, const K *Y, const K *Z,
                            std::size_t *indices, const K &tol ) {
        K radOfBox[3]; // Radius of the box
        radOfBox[0] = K( 0.5 ) * box.length( );
        radOfBox[1] = K( 0.5 ) * box.width( );
        radOfBox[2] = K( 0.5 ) * box.depth( );
        K center1[3];
        std::tie( center1[0], center1[1], center1[2] ) = box.center( );
        // Search the direction where the box is the longest.
        unsigned dirSplt = 0;
        for ( unsigned i = 1; i < 3; ++i )
            if ( radOfBox[dirSplt] < radOfBox[i] )
                dirSplt = i;
        // Compute the center of box 1
        center1[dirSplt] -= 0.5 * radOfBox[dirSplt];
        // Adapt the radius to the box 1 :
        radOfBox[dirSplt] *= 0.501;
        // Build a temporary box to sort indices :
        K minC[3], maxC[3];
        minC[0] = center1[0] - radOfBox[0];
        minC[1] = center1[1] - radOfBox[1];
        minC[2] = center1[2] - radOfBox[2];
        maxC[0] = center1[0] + radOfBox[0];
        maxC[1] = center1[1] + radOfBox[1];
        maxC[2] = center1[2] + radOfBox[2];
        Box<K>      box1( minC, maxC );
        std::size_t nbPt1 = sort<K, stride>( box1, nbPoints, X, Y, Z, indices );
        std::size_t nbPt2 = nbPoints - nbPt1;
        Box<K>      subbox1 =
            compute_boundingBox<K, stride>( nbPt1, X, Y, Z, indices, tol );
        Box<K> subbox2 = compute_boundingBox<K, stride>( nbPt2, X, Y, Z,
                                                         indices + nbPt1, tol );
        return std::make_tuple( nbPt1, subbox1, nbPt2, subbox2 );
    }
} // End Geometry namespace
// ========================================================================
