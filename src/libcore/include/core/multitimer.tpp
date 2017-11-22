#include "core/chronometer.hpp"
#include "core/multitimer.hpp"
#include <map>
#include <mutex>
#include <ostream>

namespace Core {
    // Global variable, invisible from linker
    namespace {
        std::mutex mutex_create, mutex_register, mutex_remove;
    }
    template <typename Key>
    struct MultiTimer<Key>::Implementation {
        // Unorderer map ?
        typedef std::map<Key, std::unique_ptr<Chronometer>> Container;
        Container m_chronos;
        static std::shared_ptr<MultiTimer<Key>::Implementation>
            m_pt_shared_impl;
    };
    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    // . . . . . . . . . . .
    template <typename Key>
    std::shared_ptr<typename MultiTimer<Key>::Implementation>
        MultiTimer<Key>::Implementation::m_pt_shared_impl = nullptr;
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    struct MultiTimer<Key>::const_iterator::Implementation {
        typename MultiTimer<Key>::Implementation::Container::const_iterator
            m_const_iterator;
    };
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    struct MultiTimer<Key>::iterator::Implementation {
        typename MultiTimer<Key>::Implementation::Container::iterator
            m_iterator;
    };
    // ===============================================================================================
    template <typename Key>
    MultiTimer<Key>::const_iterator::const_iterator( const MultiTimer<Key> &log,
                                                     bool end )
        : m_pt_impl( new MultiTimer<Key>::const_iterator::Implementation ) {
        if ( end ) {
            m_pt_impl->m_const_iterator = log.m_pt_impl->m_chronos.end( );
        } else {
            m_pt_impl->m_const_iterator = log.m_pt_impl->m_chronos.begin( );
        }
    }
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::const_iterator::const_iterator(
        const MultiTimer<Key>::iterator &it )
        : m_pt_impl( new MultiTimer<Key>::const_iterator::Implementation ) {
        m_pt_impl->m_const_iterator = it.m_pt_impl->m_iterator;
    }
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::const_iterator::const_iterator(
        const MultiTimer<Key>::const_iterator &it )
        : m_pt_impl( new MultiTimer<Key>::const_iterator::Implementation ) {
        m_pt_impl->m_const_iterator = it.m_pt_impl->m_const_iterator;
    }
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::const_iterator::const_iterator(
        MultiTimer<Key>::const_iterator &&it )
        : m_pt_impl( std::move( it.m_pt_impl ) ) {}
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::const_iterator::~const_iterator( ) {}
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    typename MultiTimer<Key>::const_iterator &MultiTimer<Key>::const_iterator::
    operator=( const MultiTimer<Key>::iterator &it ) {
        m_pt_impl->m_const_iterator = it.m_pt_impl->m_iterator;
        return *this;
    }
    // ...............................................................................................
    template <typename Key>
    typename MultiTimer<Key>::const_iterator &MultiTimer<Key>::const_iterator::
    operator=( const MultiTimer<Key>::const_iterator &it ) {
        if ( this != &it ) {
            m_pt_impl->m_const_iterator = it.m_pt_impl->m_const_iterator;
        }
        return *this;
    }
    // ...............................................................................................
    template <typename Key>
    typename MultiTimer<Key>::const_iterator &MultiTimer<Key>::const_iterator::
    operator=( MultiTimer<Key>::const_iterator &&it ) {
        m_pt_impl = std::move( it.m_pt_impl );
        return *this;
    }
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    const Chronometer &MultiTimer<Key>::const_iterator::operator*( ) const {
        return *( ( *m_pt_impl->m_const_iterator ).second );
    }
    // ...............................................................................................
    template <typename Key>
    bool MultiTimer<Key>::const_iterator::
    operator!=( const MultiTimer<Key>::const_iterator &it ) const {
        return m_pt_impl->m_const_iterator != it.m_pt_impl->m_const_iterator;
    }
    // ...............................................................................................
    template <typename Key>
    const Key &MultiTimer<Key>::const_iterator::getKey( ) const {
        return ( *m_pt_impl->m_const_iterator ).first;
    }
    // ...............................................................................................
    template <typename Key>
    typename MultiTimer<Key>::const_iterator &MultiTimer<Key>::const_iterator::
    operator++( ) {
        ( m_pt_impl->m_const_iterator )++;
        return *this;
    }
    // ===============================================================================================
    template <typename Key>
    MultiTimer<Key>::iterator::iterator( MultiTimer<Key> &log, bool end )
        : m_pt_impl( new MultiTimer<Key>::iterator::Implementation ) {
        if ( end ) {
            m_pt_impl->m_iterator = log.m_pt_impl->m_chronos.end( );
        } else {
            m_pt_impl->m_iterator = log.m_pt_impl->m_chronos.begin( );
        }
    }
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::iterator::iterator( const MultiTimer<Key>::iterator &it )
        : m_pt_impl( new MultiTimer<Key>::iterator::Implementation ) {
        m_pt_impl->m_iterator = it.m_pt_impl->m_iterator;
    }
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::iterator::iterator( MultiTimer<Key>::iterator &&it )
        : m_pt_impl( std::move( it.m_pt_impl ) ) {}
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::iterator::~iterator( ) {}
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    typename MultiTimer<Key>::iterator &MultiTimer<Key>::iterator::
    operator=( const MultiTimer<Key>::iterator &it ) {
        if ( this != &it ) {
            m_pt_impl->m_iterator = it.m_pt_impl->m_iterator;
        }
        return *this;
    }
    // ...............................................................................................
    template <typename Key>
    typename MultiTimer<Key>::iterator &MultiTimer<Key>::iterator::
    operator=( MultiTimer<Key>::iterator &&it ) {
        m_pt_impl = std::move( it.m_pt_impl );
        return *this;
    }
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    Chronometer &MultiTimer<Key>::iterator::operator*( ) const {
        return *( ( *m_pt_impl->m_iterator ).second );
    }
    // ...............................................................................................
    template <typename Key>
    bool MultiTimer<Key>::iterator::
    operator!=( const MultiTimer<Key>::iterator &it ) const {
        return m_pt_impl->m_iterator != it.m_pt_impl->m_iterator;
    }
    // ...............................................................................................
    template <typename Key>
    const Key &MultiTimer<Key>::iterator::getKey( ) const {
        return ( *m_pt_impl->m_iterator ).first;
    }
    // ...............................................................................................
    template <typename Key>
    typename MultiTimer<Key>::iterator &MultiTimer<Key>::iterator::
    operator++( ) {
        ( m_pt_impl->m_iterator )++;
        return *this;
    }
    // ===============================================================================================
    template <typename Key>
    MultiTimer<Key>::MultiTimer( ) {
        std::lock_guard<std::mutex> lock( mutex_create );
        if ( MultiTimer<Key>::Implementation::m_pt_shared_impl == nullptr ) {
            MultiTimer<Key>::Implementation::m_pt_shared_impl =
                std::make_shared<MultiTimer<Key>::Implementation>( );
        }
        m_pt_impl = MultiTimer<Key>::Implementation::m_pt_shared_impl;
    }
    // ...............................................................................................
    template <typename Key>
    MultiTimer<Key>::~MultiTimer( ) {}
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    void MultiTimer<Key>::unsubscribe( const Key &label ) {
        std::lock_guard<std::mutex> lock( mutex_remove );
        m_pt_impl->m_chronos.erase( label );
    }
    // ...............................................................................................
    template <typename Key>
    Chronometer &MultiTimer<Key>::operator[]( const Key &label ) {
        return *m_pt_impl->m_chronos[label];
    }
    // ...............................................................................................
    template <typename Key>
    std::ostream &MultiTimer<Key>::print( std::ostream &out ) const {
        double total_user_time = 0.;
        for ( auto it = begin( ); it != end( ); ++it ) {
            out << it.getKey( ) << " : " << *it << std::endl;
            total_user_time += ( *it ).total_time( );
        }
        out << "Total time : " << total_user_time << std::flush;
        return out;
    }
    // -----------------------------------------------------------------------------------------------
    template <typename Key>
    void MultiTimer<Key>::register_as( std::unique_ptr<Chronometer> &chrono,
                                       const Key &                   label ) {
        std::lock_guard<std::mutex> lock( mutex_register );
        m_pt_impl->m_chronos[label] = std::move( chrono );
    }
}
