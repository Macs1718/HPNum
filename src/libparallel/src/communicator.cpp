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
// Implementation of the Communicator class
#include "parallel/communicator.hpp"
#include <cassert>
#include <map>
#include "core/std_cpp_chronometer.hpp"

#if defined( USE_MPI )
#include "parallel/communicator_mpi.tpp"
#elif defined( USE_PVM )
#include "parallel/communicator_pvm.tpp"
#else
#include "parallel/communicator_stub.tpp"
#endif

namespace Parallel {
    // ------------------------------------------------------------------------
    Communicator::Chronometer::Chronometer( Communicator& com )
        : m_pt_impl( new Communicator::Chronometer::Implementation ) {
        m_pt_impl->pt_current_chronometer = nullptr;
        m_pt_impl->is_activated           = true;
        com.set_pt_chrono( this );
    }
    // ........................................................................
    Communicator::Chronometer::~Chronometer( ) {}
    // ------------------------------------------------------------------------
    Communicator::Chronometer& Communicator::Chronometer::operator[]( const std::string& label ) {
        Core::StdChronometer* pt_chronos = m_pt_impl->m_chronos[label].get( );
        if ( pt_chronos == nullptr ) {
            m_pt_impl->m_chronos[label] =
                std::move( std::unique_ptr<Core::StdChronometer>( new Core::StdChronometer ) );
            pt_chronos = m_pt_impl->m_chronos[label].get( );
        }
        m_pt_impl->pt_current_chronometer = pt_chronos;
        return *this;
    }
    // ------------------------------------------------------------------------
    const Communicator::Chronometer& Communicator::Chronometer::operator[]( const std::string& label ) const {
        assert( m_pt_impl->m_chronos[label].get( ) != nullptr );
        m_pt_impl->pt_current_chronometer = m_pt_impl->m_chronos[label].get( );
        return *this;
    }
    // ------------------------------------------------------------------------
    void Communicator::Chronometer::start_chrono( ) {
        assert( m_pt_impl->pt_current_chronometer != nullptr );
        m_pt_impl->pt_current_chronometer->start( );
    }
    // ........................................................................
    double Communicator::Chronometer::get_delta_time( ) {
        assert( m_pt_impl->pt_current_chronometer != nullptr );
        return m_pt_impl->pt_current_chronometer->stop( );
    }
    // ........................................................................
    std::ostream& Communicator::Chronometer::print( std::ostream& out ) const {
        out << "---------------->" << std::endl;
        out << "\t Communication Details : " << std::endl;
        out << "\t ===================== " << std::endl;
        for ( const auto& item : m_pt_impl->m_chronos ) {
            out << "\t\t [ " << item.first << " ] => ";
            out << *( item.second ) << std::endl;
        }
        out << "\t Communication Summaries : " << std::endl;
        out << "\t =======================" << std::endl << "\t\t";
        Core::Chronometer::print( out );
        out << "\n<----------------" << std::endl;
        return out;
    }
    // ........................................................................
    void Communicator::Chronometer::activate( ) { m_pt_impl->is_activated = true; }
    // -----------------------------------------------------------------------------
    void Communicator::Chronometer::deactivate( ) { m_pt_impl->is_activated = false; }
    // ========================================================================
    Communicator::Communicator( ) : m_impl( new Communicator::Implementation ) {
        rank = m_impl->getRank( );
        size = m_impl->getSize( );
    }
    // .............................................................................
    Communicator::Communicator( const Communicator& com, int color, int key )
        : m_impl( new Communicator::Implementation( *com.m_impl, color, key ) ) {
        rank = m_impl->getRank( );
        size = m_impl->getSize( );
    }
    // .............................................................................
    Communicator::Communicator( const Communicator& com ) : m_impl( new Communicator::Implementation( *com.m_impl ) ) {
        rank = m_impl->getRank( );
        size = m_impl->getSize( );
    }
    // .............................................................................
    Communicator::Communicator( const Ext_Communicator& excom ) : m_impl( new Communicator::Implementation( excom ) ) {
        rank = m_impl->getRank( );
        size = m_impl->getSize( );
    }
    // .............................................................................
    Communicator::~Communicator( ) { delete m_impl; }
    // =============================================================================
    int Communicator::translateRank( const Communicator& other_com ) const {
        int tr_rank;
        m_impl->translateRanks( *other_com.m_impl, 1, &rank, &tr_rank );
        return tr_rank;
    }
    // .............................................................................
    int Communicator::translateRank( const Communicator& other_com, int rk ) const {
        int tr_rank;
        m_impl->translateRanks( *other_com.m_impl, 1, &rk, &tr_rank );
        return tr_rank;
    }
    // .............................................................................
    std::vector<int> Communicator::translateRanks( const Communicator&     other_com,
                                                   const std::vector<int>& ranksToTranslate ) {
        std::vector<int> tr_ranks( ranksToTranslate.size( ) );
        m_impl->translateRanks( *other_com.m_impl, ranksToTranslate.size( ), ranksToTranslate.data( ),
                                tr_ranks.data( ) );
        return tr_ranks;
    }
    // -----------------------------------------------------------------------------
    void Communicator::set_pt_chrono( Communicator::Chronometer* pt_chrono ) { m_impl->m_pt_active_chrono = pt_chrono; }
    // =============================================================================
    void Communicator::barrier( ) const { m_impl->barrier( ); }
    // ========================================================================
    Status Communicator::probe( int source, int tag ) { return m_impl->probe( source, tag ); }
    // ------------------------------------------------------------------------------------
    bool Communicator::iprobe( Status& status, int source, int tag ) { return m_impl->iprobe( source, tag, status ); }
}
