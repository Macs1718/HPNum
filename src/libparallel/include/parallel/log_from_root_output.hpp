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
#ifndef _PARALLEL_LOG_FROM_ROOT_OUTPUT_HPP_
# define _PARALLEL_LOG_FROM_ROOT_OUTPUT_HPP_
# include "core/logger.hpp"
# include "parallel/communicator.hpp"

namespace Parallel
{
  /**
   * @brief      Logger only used by the root processus ( 0 by default, but can be redefined by the user )
   *
   *details    
   *             This class wrap a sequential logger to be runned on the root process ( usually 0 but can be redefined)
   *             The root logger is used as the wrapped logger, but is only available for the root process, other process
   *             doesn't listen for the incoming streamed messages.
   * @tparam     L     The sequential loger wrapped by this class
   */
  template<typename L> class LogFromRootOutput : public Core::Logger::Listener
  {
  public:
    template<class... Args>
    LogFromRootOutput( int flags, Args&&... args  ) :
      Core::Logger::Listener((Parallel::Communicator().rank == 0 ?
			      flags : Core::Logger::Listener::Listen_for_nothing)),
      m_listener(flags, std::forward<Args>(args)...)
    {}
    // ...............................................................................................
    template<class... Args>
    LogFromRootOutput( int flags, int root, Args&&... args  ) :
      Core::Logger::Listener((Parallel::Communicator().rank == root ?
			      flags : Core::Logger::Listener::Listen_for_nothing)),
      m_listener(flags, std::forward<Args>(args)...)
    {}
    // ...............................................................................................
    template<class... Args>
    LogFromRootOutput( int flags, Communicator& com, Args&&... args  ) :
      Core::Logger::Listener((com.rank == 0 ? flags : Core::Logger::Listener::Listen_for_nothing)),
      m_listener(flags, std::forward<Args>(args)...)
    {}
    // ...............................................................................................
    template<class... Args>
    LogFromRootOutput( int flags, Communicator& com, int root, Args&&... args  ) :
      Core::Logger::Listener((com.rank == root ? flags : Core::Logger::Listener::Listen_for_nothing)),
      m_listener(flags, std::forward<Args>(args)...)
    {}
    
  private:
    virtual std::ostream& report() override {
      return m_listener.report();
    }
    L m_listener;
  };
}

#endif
