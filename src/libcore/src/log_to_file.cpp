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
#include "core/log_to_file.hpp"

namespace Core {
    LogToFile::LogToFile( int flags, std::string const &filename ) throw(
        std::ios_base::failure ) try : Logger
        ::Listener( flags ), m_fileName( filename ) {
            m_file.open( m_fileName );
        }
    catch ( std::ios_base::failure ) {
        std::cerr << "File creation failed. This listener will be unavailable."
                  << std::endl;
    }
    // ...............................................................................................
    LogToFile::~LogToFile( ) { m_file.close( ); }
    // ...............................................................................................
    std::ostream &LogToFile::report( ) { return m_file; }
}
