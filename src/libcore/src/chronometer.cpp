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
#include "core/chronometer.hpp"

namespace Core {
    Chronometer::Chronometer( )
        : m_counter( 0UL ), m_total_time( 0. ), m_is_measuring( false ) {}
    // -----------------------------------------------------------------
    void Chronometer::start( ) {
        if ( m_is_measuring )
            return;
        m_is_measuring = true;
        start_chrono( );
    }
    // -----------------------------------------------------------------
    double Chronometer::stop( ) {
        m_is_measuring = false;
        m_counter += 1;
        double deltaT = get_delta_time( );
        m_total_time += deltaT;
        return deltaT;
    }
    // =================================================================
    double Chronometer::mean_time( ) const {
        if ( m_counter == 0 )
            return 0.;
        return m_total_time / m_counter;
    }
    // -----------------------------------------------------------------
    double Chronometer::total_time( ) const { return m_total_time; }
    // -----------------------------------------------------------------
    unsigned long Chronometer::nb_calls( ) const { return m_counter; }
    // =================================================================
    std::ostream &Chronometer::print( std::ostream &out ) const {
        out << "Time per call : " << m_total_time / m_counter
            << "\t Number of calls : " << m_counter
            << "\t Total time : " << m_total_time << "\t";
        return out;
    }
}
