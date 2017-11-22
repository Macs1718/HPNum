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
#if defined( USE_STUB )
#include "parallel/context.hpp"
#if defined( PARALLEL_TRACE )
#include "core/logger.hpp"
#endif
namespace Parallel {
    Context::Context( int &nargc, char *argv[], bool isMultithreaded )
        : Context::Context( nargc, argv,
                            ( isMultithreaded
                                  ? Context::thread_support::Multiple
                                  : Context::thread_support::Single ) ) {}
    // ...............................................................................................
    Context::Context( int &nargc, char *argv[],
                      Context::thread_support thread_level_support )
        : m_provided( thread_level_support ) {
#if defined( PARALLEL_TRACE )
        Core::Logger log;
        log << LogTrace << "Parallel context initialization with a stub "
            << std::endl;
        if ( thread_level_support == Context::thread_support::Single )
            log << "\t ---> non multithreaded context : ";
        else
            log << "\t ---> non multithreaded context : ";
#endif
    }
    // ...............................................................................................
    Context::~Context( ) {
#if defined( PARALLEL_TRACE )
        Core::Logger log;
        log << LogTrace << "Stop Parallel stub context" << std::endl;
#endif
    }
}
#endif
