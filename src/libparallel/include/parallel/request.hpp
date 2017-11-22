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
// Request
#ifndef _PARALLEL_REQUEST_HPP_
#define _PARALLEL_REQUEST_HPP_
#include "parallel/constantes.hpp"
#include "parallel/status.hpp"

#ifdef USE_MPI
#include <mpi.h>
namespace Parallel {
/**
 * @brief      Request for asynchronous message passing
 *
 */
class Request {
  public:
    /**
     * @brief      Return a new request
     */
    Request() = default;
    /**
     * @brief      Copy constructor
     *
     * @param[in]  req   The request to copy
     */
    Request(const MPI_Request &req) : m_req(req) {}
    /**
     * @brief      Test if the message is received
     *
     * @return     True if the received message is complete
     */
    bool test() {
        int flag;
        MPI_Test(&m_req, &flag, &m_status);
        return (flag != 0);
    }
    /**
     * @brief      Wait that the message is completed.
     */
    void wait() { MPI_Wait(&m_req, &m_status); }
    /**
     * @brief      Cancel the receive message
     */
    void cancel() { MPI_Cancel(&m_req); }
    /**
     * @brief      Return the status of the message
     *
     * @return     The status of the message
     */
    Status status() const { return Status(m_status); }

  private:
    MPI_Request m_req;
    MPI_Status m_status;
};
// Waitall to do, not so easy !
#elif defined(USE_PVM)
#error("Not yet implemanted");
#else
namespace Parallel {
class Request {
  public:
    Request() {}
    bool test() { return true; }
    void wait() {}
    // To think about status... Some trick to do ?
    Status status() const { return Status{.m_count = 0, .m_tag = 0, .m_error = 0}; }
};
// TO DO
#endif
}
#endif
