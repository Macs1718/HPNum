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
#ifndef _PARALLEL_LOG_FROM_DISTRIBUTED_FILE_HPP_
#define _PARALLEL_LOG_FROM_DISTRIBUTED_FILE_HPP_

#include "core/logger.hpp"
#include "core/log_to_file.hpp"
#include <cassert>
#include <fstream>
#include <memory>

namespace Parallel {
class Communicator;
/**
 * @brief      Create a log file per process with filter for warning, error and so.
 */
class LogFromDistributedFile : public Core::Logger::Listener {
  public:
    /**
     * @brief      Distributed logs on file.
     *
     * @param[in]  flags     The flags to filter the messages
     * @param[in]  basename  The basename for the files ( one file per process )
     */
    LogFromDistributedFile(int flags, const std::string &basename) throw(std::ios_base::failure);
    /**
     * @brief      Distributed logs on file.
     *
     * @param[in]  flags     The flags to filter the messages
     * @param[in]  com       The communicator from which the ranks are defined
     * @param[in]  basename  The basename of the files
     */
    LogFromDistributedFile(int flags, const Communicator &com,
                           const std::string &basename) throw(std::ios_base::failure);
    ~LogFromDistributedFile();
    virtual std::ostream &report() override {
        assert(m_pt_log.get() != nullptr);
        return m_pt_log->report();
    }

  private:
    std::unique_ptr<Core::LogToFile> m_pt_log;
};
}

#endif
