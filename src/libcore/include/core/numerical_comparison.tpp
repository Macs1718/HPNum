#include <iostream>
namespace Numerical {
    template <typename K>
    bool almost_equal( const K &a, const K &b, real_t<K> eps ) {
        if ( a == b )
            return true;
        real_t<K> diff = std::abs( a - b );
        return ( diff <= eps * std::abs( a + b ) ) ||
               ( diff < std::numeric_limits<real_t<K>>::min( ) );
    }
}
