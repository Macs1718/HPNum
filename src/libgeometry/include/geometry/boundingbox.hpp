// ========================================================================
// ==                  Boundingbox for a set of nodes                    ==
// ========================================================================
#ifndef _GEOMETRY_BOUNDINGBOX_HPP_
#define _GEOMETRY_BOUNDINGBOX_HPP_
#include "geometry/box.hpp"
#include "geometry/stride.hpp"
#include "geometry/tolerance.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>

namespace Geometry {
    template <typename K, int stride = PerComponent>
    Box<K> compute_boundingBox( const std::size_t &nbPoints, const K *X,
                                const K *Y, const K *Z,
                                const std::size_t *indices = nullptr,
                                const K &tol = Tolerance<K>::value( ) );

    // Return nbPt1, Box1, nbPt2, Box2
    template <typename K, int stride = PerComponent>
    std::tuple<std::size_t, Box<K>, std::size_t, Box<K>>
    subdivideAdjustAndSort( const Box<K> &box, const std::size_t &nbPoints,
                            const K *X, const K *Y, const K *Z,
                            std::size_t *indices,
                            const K &    tol = Tolerance<K>::value( ) );
}

#endif
