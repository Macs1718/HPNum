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
#ifndef _CORE_LOG_TO_FILE_HPP
#define _CORE_LOG_TO_FILE_HPP
#include "core/logger.hpp"
#include <fstream>

namespace Core {
    /**
     * @brief      Class for print log in a file.
     */
    class LogToFile : public Logger::Listener {
    public:
        /**
         * @brief      Logs in a file.
         *
         * @param[in]  flags     The flags to know which kind of message this
         *                       listener must manage
         * @param      filename  The filename where write the messages
         */
        LogToFile( int flags, std::string const &filename ) throw(
            std::ios_base::failure );
        /**
         * @brief      Destroy the listener
         */
        ~LogToFile( );

        /**
         * @brief      Return the output stream managed by the listener
         *
         * @return     The output stream
         */
        virtual std::ostream &report( ) override;

    private:
        std::string   m_fileName;
        std::ofstream m_file;
    };
}
#endif
