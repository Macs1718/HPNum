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
#include "core/std_cpp_chronometer.hpp"

namespace Core {
    void StdChronometer::start_chrono( ) {
        m_start = std::chrono::system_clock::now( );
    }
    // -----------------------------------------------------------------
    double StdChronometer::get_delta_time( ) {
        std::chrono::time_point<std::chrono::system_clock> end;
        end = std::chrono::system_clock::now( );
        std::chrono::duration<double> elapsed_seconds = end - m_start;
        return elapsed_seconds.count( );
    }
}
