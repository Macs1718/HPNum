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
// =================================================================================================
#ifndef _CORE_LOG_TO_STD_OUTPUT_HPP
#define _CORE_LOG_TO_STD_OUTPUT_HPP
#include "core/logger.hpp"

namespace Core {
    class LogToStdOutput : public Logger::Listener {
    public:
        LogToStdOutput( int flags );
        virtual std::ostream &report( ) override;
    };
}

#endif
