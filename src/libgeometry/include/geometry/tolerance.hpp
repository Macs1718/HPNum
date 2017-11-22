/**
 * \brief Define some numerical error tolerance constants for geometric
 * algorithms.
 *
 */
#ifndef _GEOMETRY_TOLERANCE_HPP_
#define _GEOMETRY_TOLERANCE_HPP_
#include <cmath>

namespace Geometry {
    template <typename K>
    struct Tolerance {
        constexpr static K value( ) { return K( 0 ); }
    };

    template <>
    struct Tolerance<float> {
        constexpr static float value( ) { return 1.E-3f; }
    };

    template <>
    struct Tolerance<double> {
        constexpr static double value( ) { return 1.E-6; }
    };

    template <typename K>
    static bool is_in_tolerance( const K &computed_value,
                                 const K &excepted_value ) {
        return ( std::abs( computed_value - excepted_value ) <=
                 std::abs( excepted_value ) * Tolerance<K>::value( ) );
    }
    template <typename C1, typename C2>
    static bool is_in_tolerance( const C1 &compNode, const C2 &excNode ) {
        bool isOK = true;
        isOK &= is_in_tolerance( compNode[0], excNode[0] );
        isOK &= is_in_tolerance( compNode[1], excNode[1] );
        isOK &= is_in_tolerance( compNode[2], excNode[2] );
        return isOK;
    }
    template <typename K>
    static bool
    is_in_tolerance( const std::initializer_list<K> &computed_values,
                     const std::initializer_list<K> &excepted_values ) {
        bool isOK    = true;
        auto itComp  = computed_values.begin( );
        auto itExcep = excepted_values.begin( );
        for ( ; itComp != computed_values.end( ); itComp++, itExcep++ ) {
            isOK &= is_in_tolerance( *itComp, *itExcep );
        }
        return isOK;
    }
}

#endif
