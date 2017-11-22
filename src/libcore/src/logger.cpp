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
#include "core/logger.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <mutex>

namespace Core {
    constexpr char Logger::Normal[];
    constexpr char Logger::Bright[];
    constexpr char Logger::Underline[];
    constexpr char Logger::Inverse[];
    constexpr char Logger::PrimaryFont[];
    constexpr char Logger::SecondFont[];
    constexpr char Logger::ThirdFont[];
    constexpr char Logger::FourthFont[];
    constexpr char Logger::FifthFont[];
    constexpr char Logger::SixthFont[];
    constexpr char Logger::SeventhFont[];
    constexpr char Logger::HeighthFont[];
    constexpr char Logger::NinthFont[];
    constexpr char Logger::TenthFont[];
    constexpr char Logger::NormalIntensity[];
    constexpr char Logger::NoUnderline[];
    constexpr char Logger::Black[];
    constexpr char Logger::Red[];
    constexpr char Logger::Green[];
    constexpr char Logger::Yellow[];
    constexpr char Logger::Blue[];
    constexpr char Logger::Magenta[];
    constexpr char Logger::Cyan[];
    constexpr char Logger::White[];
    constexpr char Logger::DefaultColor[];
    constexpr char Logger::BBlack[];
    constexpr char Logger::BRed[];
    constexpr char Logger::BGreen[];
    constexpr char Logger::BYellow[];
    constexpr char Logger::BBlue[];
    constexpr char Logger::BMagenta[];
    constexpr char Logger::BCyan[];
    constexpr char Logger::BWhite[];
    constexpr char Logger::DefaultBackground[];
    constexpr char Logger::Framed[];
    constexpr char Logger::Encircled[];
    constexpr char Logger::Overlined[];
    constexpr char Logger::NoFramed[];
    // Global variable, invisible from linker
    namespace {
        std::mutex mutex_create, mutex_register, mutex_remove;
    }

    // Logger Implementation :
    struct Logger::Implementation {
        typedef std::list<Listener *> Container;
        Implementation( ) : m_current_mode( Logger::information ), m_listeners( ) {}
        int       m_current_mode;
        Container m_listeners;

        static std::shared_ptr<Logger::Implementation> m_pt_shared_impl;
    };

    std::shared_ptr<Logger::Implementation> Logger::Implementation::m_pt_shared_impl = nullptr;
    // ...............................................................................................
    // Logger iterator implementation
    struct Logger::iterator::Implementation {
        typedef Logger::Implementation::Container::iterator iterator_impl;
        iterator_impl                                       m_iterator;
    };
    // ...............................................................................................
    // Logger const_iterator implementation
    struct Logger::const_iterator::Implementation {
        typedef Logger::Implementation::Container::const_iterator const_iterator_impl;
        const_iterator_impl                                       m_const_iterator;
    };
    // ===============================================================================================
    // Logger iterator definition :
    Logger::iterator::iterator( Logger &log, bool end ) : m_pt_impl( new Logger::iterator::Implementation ) {
        if ( end ) {
            m_pt_impl->m_iterator = log.m_pt_impl->m_listeners.end( );
        } else {
            m_pt_impl->m_iterator = log.m_pt_impl->m_listeners.begin( );
        }
    }
    // ...............................................................................................
    Logger::iterator::iterator( const Logger::iterator &it ) : m_pt_impl( new Logger::iterator::Implementation ) {
        m_pt_impl->m_iterator = it.m_pt_impl->m_iterator;
    }
    // ...............................................................................................
    Logger::iterator::iterator( Logger::iterator &&it ) : m_pt_impl( std::move( it.m_pt_impl ) ) {}
    // ...............................................................................................
    Logger::iterator::~iterator( ) {}
    // ...............................................................................................
    Logger::iterator &Logger::iterator::operator=( const Logger::iterator &it ) {
        if ( this != &it ) { m_pt_impl->m_iterator = it.m_pt_impl->m_iterator; }
        return *this;
    }
    // ...............................................................................................
    Logger::iterator &Logger::iterator::operator=( Logger::iterator &&it ) {
        m_pt_impl = std::move( it.m_pt_impl );
        return *this;
    }
    // ...............................................................................................
    bool Logger::iterator::operator!=( const Logger::iterator &it ) const {
        return m_pt_impl->m_iterator != it.m_pt_impl->m_iterator;
    }
    // ...............................................................................................
    Logger::Listener *Logger::iterator::operator*( ) const { return *m_pt_impl->m_iterator; }
    // ...............................................................................................
    Logger::iterator &Logger::iterator::operator++( ) {
        ( m_pt_impl->m_iterator )++;
        return *this;
    }
    // -----------------------------------------------------------------------------------------------
    Logger::const_iterator::const_iterator( const Logger &log, bool end )
        : m_pt_impl( new Logger::const_iterator::Implementation ) {
        if ( end ) {
            m_pt_impl->m_const_iterator = log.m_pt_impl->m_listeners.end( );
        } else {
            m_pt_impl->m_const_iterator = log.m_pt_impl->m_listeners.begin( );
        }
    }
    // ...............................................................................................
    Logger::const_iterator::const_iterator( const Logger::iterator &it )
        : m_pt_impl( new Logger::const_iterator::Implementation ) {
        m_pt_impl->m_const_iterator = it.m_pt_impl->m_iterator;
    }
    // ...............................................................................................
    Logger::const_iterator::const_iterator( const Logger::const_iterator &it )
        : m_pt_impl( new Logger::const_iterator::Implementation ) {
        m_pt_impl->m_const_iterator = it.m_pt_impl->m_const_iterator;
    }
    // ...............................................................................................
    Logger::const_iterator::const_iterator( Logger::const_iterator &&it ) : m_pt_impl( std::move( it.m_pt_impl ) ) {}
    // ...............................................................................................
    Logger::const_iterator::~const_iterator( ) {}
    // ...............................................................................................
    Logger::const_iterator &Logger::const_iterator::operator=( const Logger::const_iterator &it ) {
        if ( this != &it ) { m_pt_impl->m_const_iterator = it.m_pt_impl->m_const_iterator; }
        return *this;
    }
    // ...............................................................................................
    Logger::const_iterator &Logger::const_iterator::operator=( Logger::const_iterator &&it ) {
        m_pt_impl = std::move( it.m_pt_impl );
        return *this;
    }
    // ...............................................................................................
    bool Logger::const_iterator::operator!=( const Logger::const_iterator &it ) const {
        return m_pt_impl->m_const_iterator != it.m_pt_impl->m_const_iterator;
    }
    // ...............................................................................................
    const Logger::Listener *Logger::const_iterator::operator*( ) const { return *m_pt_impl->m_const_iterator; }
    // ...............................................................................................
    Logger::const_iterator &Logger::const_iterator::operator++( ) {
        ( m_pt_impl->m_const_iterator )++;
        return *this;
    }
    // -----------------------------------------------------------------------------------------------
    Logger::Logger( ) : m_pt_impl( nullptr ) {
        std::lock_guard<std::mutex> lock( mutex_create );
        if ( Logger::Implementation::m_pt_shared_impl == nullptr )
            Logger::Implementation::m_pt_shared_impl = std::make_shared<Logger::Implementation>( );
        m_pt_impl                                    = Logger::Implementation::m_pt_shared_impl;
    }
    // ...............................................................................................
    bool Logger::subscribe( Logger::Listener *listener ) {
        std::lock_guard<std::mutex> lock( mutex_register );
        if ( listener == nullptr ) return false;
        auto itL = std::find( m_pt_impl->m_listeners.begin( ), m_pt_impl->m_listeners.end( ), listener );
        if ( itL != m_pt_impl->m_listeners.end( ) ) return false;
        m_pt_impl->m_listeners.push_back( listener );
        return true;
    }
    // ...............................................................................................
    bool Logger::unsubscribe( Logger::Listener *listener ) {
        std::lock_guard<std::mutex> lock( mutex_remove );
        if ( listener == NULL ) return false;
        auto itL = std::find( m_pt_impl->m_listeners.begin( ), m_pt_impl->m_listeners.end( ), listener );
        if ( itL == m_pt_impl->m_listeners.end( ) ) return false;
        m_pt_impl->m_listeners.remove( listener );
        return true;
    }
    // ...............................................................................................
    Logger &Logger::operator[]( int mode ) {
        m_pt_impl->m_current_mode = mode;
        return *this;
    }
    // ...............................................................................................
    Logger &Logger::set_mode( int mode ) {
        m_pt_impl->m_current_mode = mode;
        return *this;
    }
    // ...............................................................................................
    int Logger::get_mode( ) const { return m_pt_impl->m_current_mode; }
    // -----------------------------------------------------------------------------------------------
    Logger::Listener::Listener( int flags ) : m_flags( flags ) {}
    // ...............................................................................................
    Logger::Listener::~Listener( ) {}
}
