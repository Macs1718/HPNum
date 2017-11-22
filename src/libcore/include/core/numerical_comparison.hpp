// Copyright 2017 Dr. Xavier JUVIGNY

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef _CORE_NUMERICAL_COMPARISON_HPP_
#define _CORE_NUMERICAL_COMPARISON_HPP_
#include <limits>

namespace Numerical {
    template <typename K>
    using real_t = decltype( std::abs( K( 0 ) ) );

    template <typename K>
    bool
    almost_equal( const K &a, const K &b,
                  real_t<K> eps = 100 *
                                  std::numeric_limits<real_t<K>>::epsilon( ) );
}

#endif
