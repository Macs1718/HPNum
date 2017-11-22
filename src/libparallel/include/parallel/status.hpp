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
// Status des réceptions/envois non synchros
#ifndef _PARALLEL_STATUS_HPP_
#define _PARALLEL_STATUS_HPP_
#ifdef USE_MPI
#include <mpi.h>
#endif
#include "parallel/constantes.hpp"
namespace Parallel {
#if defined(USE_MPI)
/**
 * @brief      Status of a message ( complete, pending, and so .)
 */
struct Status {
  public:
    /**
     * @brief      Create a new status instance
     */
    Status() = default;
    /**
     * @brief      Create a new status instance from MPI status
     *
     * @param[in]  st    a MPI status to convert in parallel instance
     */
    Status(MPI_Status st) : status(st) {}

    MPI_Status status;

    /**
     * @brief      Return the number of elements contained in the received message
     *
     * @tparam     K     The kind of element to receive ( int, double, and so )
     *
     * @return     The number of elements
     */
    template <typename K>
    int count() const {
        int cnt;
        MPI_Get_count(&status, Type_MPI<K>::mpi_type(), &cnt);
        return cnt;
    }
    /**
     * @brief      Return the sender ( or receiver ) rank for the message
     *
     * @return     { description_of_the_return_value }
     */
    int source() const { return status.MPI_SOURCE; }
    /**
     * @brief      Return the id tag for the received ( or sended ) message
     *
     * @return     The id tag
     */
    int tag() const { return status.MPI_TAG; }
    /**
     * @brief      Return the error status for the received ( or sended ) message
     *
     * @return     The error code ( error::success for none error )
     */
    int error() const { return status.MPI_ERROR; }
};
#elif defined(USE_PVM)
#error("Not yet implemanted");
#else
/*!
 *    \struct Status
 *    \brief  The state of an incoming message
 *
 */
struct Status {
    /*!
     *    \brief Return the number of objects contained in the incoming message
     */
    template <typename K>
    int count() const {
        return m_count;
    }
    /*!
     *    \brief Return the identity tag of the incoming message
     */
    int tag() const { return m_tag; }
    /*!
     *    \brief Return the rank of the sender of the incoming message
     */
    int source() const { return 0; }
    /*!
     *    \brief Return the error state of the incoming message.
     */
    int error() const { return m_error; }
    /// \privatesection
    int m_count, m_tag, m_error;
};
#endif
}
#endif
