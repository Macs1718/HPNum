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
#if defined( USE_MPI )
#include "parallel/context.hpp"
#include <mpi.h>
#if defined( PARALLEL_TRACE )
#include "core/logger.hpp"
#endif
namespace Parallel {
    Communicator *Context::pt_global_com = nullptr;
    Context::Context( int &nargc, char *argv[], bool isMultithreaded )
        : Context::Context(
              nargc, argv, ( isMultithreaded ? Context::thread_support::Multiple : Context::thread_support::Single ) ) {
    }
    // ...............................................................................................
    Context::Context( int &nargc, char *argv[], Context::thread_support thread_level_support )
        : m_provided( thread_level_support ) {
#if defined( PARALLEL_TRACE )
        Core::Logger log;
        log << LogTrace << "Parallel context initialization with MPI" << std::endl;
#endif
        if ( thread_level_support == Context::thread_support::Single )
            MPI_Init( &nargc, &argv );
        else {
#if defined( PARALLEL_TRACE )
            log << "\t ---> contexte multithreadé : ";
#endif
            int level_support;
            switch ( thread_level_support ) {
            case thread_support::Funneled:
                level_support = MPI_THREAD_FUNNELED;
                break;
            case thread_support::Serialized:
                level_support = MPI_THREAD_SERIALIZED;
                break;
            default:
                level_support = MPI_THREAD_MULTIPLE;
            }
            int provided;
            MPI_Init_thread( &nargc, &argv, MPI_THREAD_MULTIPLE, &provided );
#if defined( PARALLEL_TRACE )
            log << " Compability level : " << provided << std::endl;
#endif
            if ( provided < MPI_THREAD_SERIALIZED )
                throw std::runtime_error(
                    "Not found multithreaded mode for "
                    "the current MPI library" );
            switch ( provided ) {
            case MPI_THREAD_FUNNELED:
                m_provided = Context::thread_support::Funneled;
                break;
            case MPI_THREAD_SERIALIZED:
                m_provided = Context::thread_support::Serialized;
                break;
            default:
                m_provided = Context::thread_support::Multiple;
            }
        }
    }
    // ...............................................................................................
    Context::~Context( ) {
#if defined( PARALLEL_TRACE )
        Core::Logger log;
        log << LogTrace << "Stop MPI context" << std::endl;
#endif
        MPI_Finalize( );
    }
    // ...............................................................................................
    const Communicator &Context::globalCommunicator( ) {
        if ( pt_global_com == nullptr ) pt_global_com = new Communicator;
        return *pt_global_com;
    }
}
#endif
