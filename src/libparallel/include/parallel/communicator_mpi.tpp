//========================================================================
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
// ========================================================================
// Communicator for MPI
#include "core/detect_container.hpp"
#include "core/logger.hpp"
#include "core/multitimer.hpp"
#include "parallel/chronometer_implementation.hpp"
#include "parallel/constantes.hpp"
#include "parallel/context.hpp"
#include "parallel/status.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <mpi.h>

#define BEGIN_PROFILE_COMMUNICATION                                                               \
    if (m_pt_active_chrono != nullptr) {                                                          \
        if (m_pt_active_chrono->m_pt_impl->is_activated) (*m_pt_active_chrono)[__func__].start(); \
    }

#define END_PROFILE_COMMUNICATION                                                                \
    if (m_pt_active_chrono != nullptr) {                                                         \
        if (m_pt_active_chrono->m_pt_impl->is_activated) (*m_pt_active_chrono)[__func__].stop(); \
    }

namespace Parallel {
namespace {
template <typename K>
std::function<K(const K &, const K &)> reduce_functor;
// ay <= accumulator
template <typename K>
void reduce_user_function(void *x, void *y, int *length, MPI_Datatype *tp) {
    K val;
    K *ax = (K *)x;
    K *ay = (K *)y;
    for (int i = 0; i < *length; ++i) {
        val   = reduce_functor<K>(ax[i], ay[i]);
        ay[i] = val;
    }
}
}
// #################################################################################################
struct Communicator::Implementation {
    Implementation() : m_pt_active_chrono(nullptr) { MPI_Comm_dup(MPI_COMM_WORLD, &m_communicator); }
    // ...............................................................................................
    Implementation(const Implementation &impl, int color, int key) : m_pt_active_chrono(nullptr) {
        MPI_Comm_split(impl.m_communicator, color, key, &m_communicator);
    }
    // ...............................................................................................
    Implementation(const Implementation &impl) : m_pt_active_chrono(nullptr) {
        MPI_Comm_dup(impl.m_communicator, &m_communicator);
    }
    // ...............................................................................................
    Implementation(const Ext_Communicator &excom) : m_pt_active_chrono(nullptr) {
        MPI_Comm_dup(excom, &m_communicator);
    }
    // ...............................................................................................
    ~Implementation() { MPI_Comm_free(&m_communicator); }
    // -----------------------------------------------------------------------------------------------
    int getRank() const {
        int rank;
        MPI_Comm_rank(m_communicator, &rank);
        return rank;
    }
    // ...............................................................................................
    void translateRanks(Communicator::Implementation &o_impl, int nbRanks, const int *ranks, int *tr_ranks) const {
        MPI_Group group1, group2;
        MPI_Comm_group(m_communicator, &group1);
        MPI_Comm_group(o_impl.m_communicator, &group2);
        MPI_Group_translate_ranks(group1, nbRanks, ranks, group2, tr_ranks);
    }
    // ...............................................................................................
    int getSize() const {
        int size;
        MPI_Comm_size(m_communicator, &size);
        return size;
    }
    // ...............................................................................................
    const Ext_Communicator &get_ext_comm() const { return m_communicator; }
    // ...............................................................................................
    Status probe(int src, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        Status status;
        MPI_Probe(src, tag, m_communicator, &status.status);
        END_PROFILE_COMMUNICATION
        return status;
    }
    // ===============================================================================================
    // For communication, we handle container in special functions. The flag
    // here tells if the object
    // is a container or not. See below for the specialization of this
    // structure for containers
    // ( specialization of the functions mainly ).
    template <typename K, bool flag = false>
    struct Communication {
        static void send(const MPI_Comm &com, const K &snd_obj, int dest, int tag) {
            if (Type_MPI<K>::must_be_packed()) {
                MPI_Send(&snd_obj, sizeof(K), MPI_BYTE, dest, tag, com);
            } else {
                MPI_Send(&snd_obj, 1, Type_MPI<K>::mpi_type(), dest, tag, com);
            }
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Send an object to " << dest << " with tag " << tag << "\n";
#endif
        }
        // .......................................................................................
        static Request isend(const MPI_Comm &com, const K &snd_obj, int dest, int tag) {
            MPI_Request m_req;
            if (Type_MPI<K>::must_be_packed()) {
                MPI_Isend(&snd_obj, sizeof(K), MPI_BYTE, dest, tag, com, &m_req);
            } else {
                MPI_Isend(&snd_obj, 1, Type_MPI<K>::mpi_type(), dest, tag, com, &m_req);
            }
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Asynchrone send for an object to " << dest << " with tag " << tag << std::endl;
#endif
            return Request(m_req);
        }
        // .......................................................................................
        static Status recv(const MPI_Comm &com, K &rcvobj, int sender, int tag) {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Receive an object from " << sender << " with tag " << tag << std::endl;
#endif
            Status status;
            if (Type_MPI<K>::must_be_packed()) {
                MPI_Recv(&rcvobj, sizeof(K), MPI_BYTE, sender, tag, com, &status.status);
            } else {
                MPI_Recv(&rcvobj, 1, Type_MPI<K>::mpi_type(), sender, tag, com, &status.status);
            }
#if defined(PARALLEL_TRACE)
            log << LogTrace << "OK, receive done !" << std::endl;
#endif
            return status;
        }
        // .......................................................................................
        static Request irecv(const MPI_Comm &com, K &rcvobj, int sender, int tag) {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Asynchronous receive of an object from " << sender << " with tag " << tag << std::endl;
#endif
            MPI_Request req;
            if (Type_MPI<K>::must_be_packed()) {
                MPI_Irecv(&rcvobj, sizeof(K), MPI_BYTE, sender, tag, com, &req);
            } else {
                MPI_Irecv(&rcvobj, 1, Type_MPI<K>::mpi_type(), sender, tag, com, &req);
            }
            return Request(req);
        }
        // .......................................................................................
        static void broadcast(const MPI_Comm &com, const K *obj_snd, K &obj_rcv, int root) {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Broadcast of one object with root = " << root << std::endl;
#endif
            int rank;
            MPI_Comm_rank(com, &rank);
            if (root == rank) {
                assert(obj_snd != nullptr);
                obj_rcv = *obj_snd;
            }
            if (Type_MPI<K>::must_be_packed()) {
                MPI_Bcast(&obj_rcv, sizeof(K), MPI_BYTE, root, com);
            } else {
                MPI_Bcast(&obj_rcv, 1, Type_MPI<K>::mpi_type(), root, com);
            }
        }
        // .......................................................................................
        static void reduce(const MPI_Comm &com, const K &loc, K *glob, const Operation &op, int root) {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Reduce operation on one object with root = " << root << " and store at adress "
                << (void *)glob << std::endl;
#endif
#if defined(DEBUG)
            int rank;
            MPI_Comm_rank(com, &rank);
            assert((rank != root) || (glob != nullptr));
#endif
            MPI_Reduce(&loc, glob, 1, Type_MPI<K>::mpi_type(), op, root, com);
#if defined(PARALLEL_TRACE)
            log << LogTrace << "End of reduction" << std::endl;
#endif
        }
        // .......................................................................................
        static void allreduce(const MPI_Comm &com, const K &loc, K *glob, const Operation &op) {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "AllReduce operation on one object, store at adress " << (void *)glob << std::endl;
#endif
            MPI_Allreduce(&loc, glob, 1, Type_MPI<K>::mpi_type(), op, com);
#if defined(PARALLEL_TRACE)
            log << LogTrace << "End of all reduction" << std::endl;
#endif
        }
    };
    // ---------------------------------------------------------------------------------------------
    // General send if above methods don't work :
    template <typename K>
    void send(std::size_t nbItems, const K *sndbuff, int dest, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        if (Type_MPI<K>::must_be_packed()) {
            MPI_Send(sndbuff, nbItems * sizeof(K), MPI_BYTE, dest, tag, m_communicator);
        } else {
            MPI_Send(sndbuff, nbItems, Type_MPI<K>::mpi_type(), dest, tag, m_communicator);
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Send a message using buffer at " << (void *)sndbuff << " for " << dest << " with tag "
                << tag << " containing " << nbItems << " items." << std::endl;
#endif
        }
        END_PROFILE_COMMUNICATION
    }
    // ...........................................................................................
    template <typename K>
    void send(const K &snd, int dest, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        Communication<K, is_container<K>::value>::send(m_communicator, snd, dest, tag);
        END_PROFILE_COMMUNICATION
    }
    // -------------------------------------------------------------------------------------------
    template <typename K>
    Request isend(std::size_t nbItems, const K *sndbuff, int dest, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        MPI_Request m_req;
        if (Type_MPI<K>::must_be_packed()) {
            MPI_Isend(sndbuff, nbItems * sizeof(K), MPI_BYTE, dest, tag, m_communicator, &m_req);
        } else {
            MPI_Isend(sndbuff, nbItems, Type_MPI<K>::mpi_type(), dest, tag, m_communicator, &m_req);
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << Core::Logger::Blue << "Sended a buffer at adress " << (void *)sndbuff << " for " << dest
                << " with tag " << tag << " containing " << nbItems << " items" << Core::Logger::DefaultColor
                << std::endl;
#endif
        }
        END_PROFILE_COMMUNICATION
        return Request(m_req);
    }
    // .........................................................................................
    template <typename K>
    Request isend(const K &snd, int dest, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        Request req = Communication<K, is_container<K>::value>::isend(m_communicator, snd, dest, tag);
        END_PROFILE_COMMUNICATION
        return req;
    }
    // -------------------------------------------------------------------------------------------
    // Default receive :
    template <typename K>
    Status recv(std::size_t nbItems, K *rcvbuff, int sender, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        Status status;
        if (Type_MPI<K>::must_be_packed()) {
            MPI_Recv(rcvbuff, nbItems * sizeof(K), MPI_BYTE, sender, tag, m_communicator, &status.status);
        } else {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << Core::Logger::Magenta << "Receive a buffer at adress " << (void *)rcvbuff
                << " coming from " << sender << " with tag " << tag << " containing " << nbItems << " items."
                << Core::Logger::DefaultColor << std::endl;
#endif
            MPI_Recv(rcvbuff, nbItems, Type_MPI<K>::mpi_type(), sender, tag, m_communicator, &status.status);
#if defined(PARALLEL_TRACE)
            log << LogTrace << "Receive ok !" << std::endl;
#endif
        }
        END_PROFILE_COMMUNICATION
        return status;
    }
    // .........................................................................................
    template <typename K>
    Status recv(K &rcvobj, int sender, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        Status stat = Communication<K, is_container<K>::value>::recv(m_communicator, rcvobj, sender, tag);
        END_PROFILE_COMMUNICATION
        return stat;
    }
    // -------------------------------------------------------------------------------------------
    template <typename K>
    Request irecv(std::size_t nbItems, K *rcvbuff, int sender, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        MPI_Request req;
        if (Type_MPI<K>::must_be_packed()) {
            MPI_Irecv(rcvbuff, nbItems * sizeof(K), MPI_BYTE, sender, tag, m_communicator, &req);
        } else {
#if defined(PARALLEL_TRACE)
            Core::Logger log;
            log << LogTrace << "Receive a buffer at adress " << (void *)rcvbuff
                << " with non blocking message coming from " << sender << " with tag " << tag << " containing "
                << nbItems << " items." << std::endl;
#endif
            MPI_Irecv(rcvbuff, nbItems, Type_MPI<K>::mpi_type(), sender, tag, m_communicator, &req);
        }
        END_PROFILE_COMMUNICATION
        return Request(req);
    }
    // .........................................................................................
    template <typename K>
    Request irecv(K &rcvobj, int sender, int tag) const {
        BEGIN_PROFILE_COMMUNICATION
        Request req = Communication<K, is_container<K>::value>::irecv(m_communicator, rcvobj, sender, tag);
        END_PROFILE_COMMUNICATION
        return req;
    }
    // -----------------------------------------------------------------------------------------
    // Basic Broadcast :
    template <typename K>
    void broadcast(std::size_t nbItems, const K *bufsnd, K *bufrcv, int root) const {
        BEGIN_PROFILE_COMMUNICATION
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << Core::Logger::Cyan << "Broadcast " << nbItems << " objects at adress " << (void *)bufsnd
            << " to adress " << (void *)bufrcv << " with root = " << root << Core::Logger::Normal << std::endl;
#endif
        MPI_Barrier(m_communicator);
        assert(bufrcv != nullptr);
        if (root == getRank()) {
            assert(bufsnd != nullptr);
            if (bufsnd != bufrcv) {
#if defined(PARALLEL_TRACE)
                log << "Copy data in recv buffer for root" << root << std::endl;
#endif
                std::copy_n(bufsnd, nbItems, bufrcv);
            }
        }
        if (Type_MPI<K>::must_be_packed()) {
            MPI_Bcast(bufrcv, nbItems * sizeof(K), MPI_BYTE, root, m_communicator);
        } else {
#if defined(PARALLEL_TRACE)
            log << "Unpacked buffer to broadcast : " << nbItems << " data with root : " << root << std::endl;
#endif
            MPI_Bcast(bufrcv, nbItems, Type_MPI<K>::mpi_type(), root, m_communicator);
        }
        MPI_Barrier(m_communicator);
        END_PROFILE_COMMUNICATION
    }
    // .........................................................................................
    template <typename K>
    void broadcast(const K *obj_snd, K &obj_rcv, int root) const {
        BEGIN_PROFILE_COMMUNICATION
        Communication<K, is_container<K>::value>::broadcast(m_communicator, obj_snd, obj_rcv, root);
        END_PROFILE_COMMUNICATION
    }
    // -----------------------------------------------------------------------------------------
    template <typename K>
    Request ibroadcast(std::size_t nbItems, const K *bufsnd, K *bufrcv, int root) const {
        BEGIN_PROFILE_COMMUNICATION
        assert(bufrcv != nullptr);
        if (root == getRank()) {
            assert(bufsnd != nullptr);
            if (bufsnd != bufrcv) std::copy_n(bufsnd, nbItems, bufrcv);
        }
        Request req;
        MPI_Ibcast(bufrcv, nbItems, Type_MPI<K>::mpi_type(), root, m_communicator, &req);
        END_PROFILE_COMMUNICATION
        return req;
    }
    // -----------------------------------------------------------------------------------------
    void barrier() const {
        BEGIN_PROFILE_COMMUNICATION
        MPI_Barrier(m_communicator);
        END_PROFILE_COMMUNICATION
    }
    // ----------------------------------------------------------------------------------------------------
    Status probe(int source, int tag) {
        BEGIN_PROFILE_COMMUNICATION
        Status status;
        MPI_Probe(source, tag, m_communicator, &status.status);
        END_PROFILE_COMMUNICATION
        return status;
    }
    // ----------------------------------------------------------------------------------------------------
    bool iprobe(int source, int tag, Status &status) {
        BEGIN_PROFILE_COMMUNICATION
        int flag;
        MPI_Iprobe(source, tag, m_communicator, &flag, &status.status);
        END_PROFILE_COMMUNICATION
        return (flag != 0);
    }
    // -----------------------------------------------------------------------------------------
    template <typename K>
    void reduce(std::size_t nbItems, const K *objs, K *res, Operation op, int root) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << Core::Logger::Cyan << " Reduce operation for " << nbItems << " items with root " << root
            << std::endl;
#endif
        BEGIN_PROFILE_COMMUNICATION
        assert(objs != nullptr);
        if (root == getRank()) {
            assert(res != nullptr);
            if (objs == res) {
                MPI_Reduce(MPI_IN_PLACE, res, nbItems, Type_MPI<K>::mpi_type(), op, root, m_communicator);
            } else {
                MPI_Reduce(objs, res, nbItems, Type_MPI<K>::mpi_type(), op, root, m_communicator);
            }
        }
        END_PROFILE_COMMUNICATION
    }
    // .........................................................................................
    template <typename K>
    void reduce(const K &loc, K *glob, const Operation &op, int root) const {
        BEGIN_PROFILE_COMMUNICATION
        Communication<K, is_container<K>::value>::reduce(m_communicator, loc, glob, op, root);
        END_PROFILE_COMMUNICATION
    }
    // -----------------------------------------------------------------------------------------
    template <typename K, typename F>
    void reduce(std::size_t nbItems, const K *objs, K *res, const F &fct, bool commute, int root) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << Core::Logger::Cyan << " Reduce operation for " << nbItems << " items with root " << root
            << std::endl;
#endif
        BEGIN_PROFILE_COMMUNICATION
        assert(objs != nullptr);
        reduce_functor<K> = std::function<K(const K &, const K &)>(fct);

        MPI_Op op;
        MPI_Op_create(reduce_user_function<K>, (commute ? 1 : 0), &op);

        if (root == getRank()) {
            assert(res != nullptr);
            if (objs == res) {
                MPI_Reduce(MPI_IN_PLACE, res, nbItems, Type_MPI<K>::mpi_type(), op, root, m_communicator);
            } else {
                MPI_Reduce(objs, res, nbItems, Type_MPI<K>::mpi_type(), op, root, m_communicator);
            }
        } else
            MPI_Reduce(objs, res, nbItems, Type_MPI<K>::mpi_type(), op, root, m_communicator);
        MPI_Op_free(&op);
        END_PROFILE_COMMUNICATION
    }
    // .........................................................................................
    template <typename K, typename F>
    void reduce(const K &loc, K *glob, const F &fct, bool is_commuting, int root) const {
        reduce_functor<K> = std::function<K(const K &, const K &)>(fct);

        MPI_Op op;
        MPI_Op_create(reduce_user_function<K>, (is_commuting ? 1 : 0), &op);

        Communication<K, is_container<K>::value>::reduce(m_communicator, loc, glob, op, root);

        MPI_Op_free(&op);
    }
    // ====================================================================================================
    template <typename K>
    void allreduce(std::size_t nbItems, const K *objs, K *res, Operation op) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << Core::Logger::Cyan << "AllReduce operation on " << nbItems << " objects, stored at adress "
            << (void *)objs << Core::Logger::Normal << std::endl;
#endif
        BEGIN_PROFILE_COMMUNICATION
        assert(objs != nullptr);
        if (objs == res) {
            MPI_Allreduce(MPI_IN_PLACE, res, nbItems, Type_MPI<K>::mpi_type(), op, m_communicator);
        } else {
            MPI_Allreduce(objs, res, nbItems, Type_MPI<K>::mpi_type(), op, m_communicator);
        }
        END_PROFILE_COMMUNICATION
    }
    // .........................................................................................
    template <typename K>
    void allreduce(const K &loc, K *glob, const Operation &op) const {
        BEGIN_PROFILE_COMMUNICATION
        Communication<K, is_container<K>::value>::allreduce(m_communicator, loc, glob, op);
        END_PROFILE_COMMUNICATION
    }
    // -----------------------------------------------------------------------------------------
    template <typename K, typename F>
    void allreduce(std::size_t nbItems, const K *objs, K *res, const F &fct, bool commute) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << Core::Logger::Cyan << "AllReduce operation on " << nbItems << " objects, stored at adress "
            << (void *)objs << Core::Logger::Normal << std::endl;
#endif
        BEGIN_PROFILE_COMMUNICATION
        assert(objs != nullptr);
        reduce_functor<K> = std::function<K(const K &, const K &)>(fct);

        MPI_Op op;
        MPI_Op_create(reduce_user_function<K>, (commute ? 1 : 0), &op);

        assert(res != nullptr);
        if (objs == res) {
            MPI_Allreduce(MPI_IN_PLACE, res, nbItems, Type_MPI<K>::mpi_type(), op, m_communicator);
        } else {
            MPI_Allreduce(objs, res, nbItems, Type_MPI<K>::mpi_type(), op, m_communicator);
        }
        MPI_Op_free(&op);
        END_PROFILE_COMMUNICATION
    }
    // .........................................................................................
    template <typename K, typename F>
    void allreduce(const K &loc, K *glob, const F &fct, bool is_commuting) const {
        reduce_functor<K> = std::function<K(const K &, const K &)>(fct);

        MPI_Op op;
        MPI_Op_create(reduce_user_function<K>, (is_commuting ? 1 : 0), &op);

        Communication<K, is_container<K>::value>::allreduce(m_communicator, loc, glob, op);

        MPI_Op_free(&op);
    }
    // ===============================================================================================
    Communicator::Chronometer *m_pt_active_chrono;
    bool m_is_active_chrono;

  private:
    MPI_Comm m_communicator;
};
// ###############################################################################################
// # Specialization of communication functions for containers :
template <typename K>
struct Communicator::Implementation::Communication<K, true> {
    static void send(const MPI_Comm &com, const K &snd_arr, int dest, int tag) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
#endif
        std::vector<typename K::value_type, typename K::allocator_type> *snd;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            snd = (std::vector<typename K::value_type, typename K::allocator_type> *)&snd_arr;
        } else {
#if defined(PARALLEL_TRACE)
            log << "Copy container data inside a vector" << std::endl;
#endif
            snd = new std::vector<typename K::value_type, typename K::allocator_type>(snd_arr.size());
            std::copy(snd_arr.begin(), snd_arr.end(), snd->begin());
        }

        if (Type_MPI<typename K::value_type>::must_be_packed()) {
            MPI_Send(snd->data(), snd->size() * sizeof(typename K::value_type), MPI_BYTE, dest, tag, com);
        } else {
            MPI_Send(snd->data(), snd->size(), Type_MPI<typename K::value_type>::mpi_type(), dest, tag, com);
        }
#if defined(PARALLEL_TRACE)
        log << "Send a container with " << snd->size() << " elements to " << dest << " with tag " << tag << std::endl;
#endif
        if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
#if defined(PARALLEL_TRACE)
            log << "Delete temporary vector" << std::endl;
#endif
            delete snd;
        }
    }
    // .......................................................................................
    static Request isend(const MPI_Comm &com, const K &snd_obj, int dest, int tag) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
#endif
        std::vector<typename K::value_type, typename K::allocator_type> *snd;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            snd = (std::vector<typename K::value_type, typename K::allocator_type> *)&snd_obj;
        } else {
#if defined(PARALLEL_TRACE)
            log << "Copy container data inside a vector" << std::endl;
#endif
            snd = new std::vector<typename K::value_type, typename K::allocator_type>(snd_obj.size());
            std::copy(snd_obj.begin(), snd_obj.end(), snd->begin());
        }

        MPI_Request m_req;
        if (Type_MPI<typename K::value_type>::must_be_packed()) {
            MPI_Isend(snd->data(), snd->size() * sizeof(typename K::value_type), MPI_BYTE, dest, tag, com, &m_req);
        } else {
            MPI_Isend(snd->data(), snd->size(), Type_MPI<typename K::value_type>::mpi_type(), dest, tag, com, &m_req);
        }
#if defined(PARALLEL_TRACE)
        log << "Asynchrone send for a container with " << snd_obj.size() << " elements  to " << dest << " with tag "
            << tag << std::endl;
#endif

        if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
#if defined(PARALLEL_TRACE)
            log << "Delete temporary vector" << std::endl;
#endif
            delete snd;
        }
        return Request(m_req);
    }
    // .......................................................................................
    static Status recv(const MPI_Comm &com, K &rcvobj, int sender, int tag) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
#endif
        MPI_Status status;
        MPI_Probe(sender, tag, com, &status);
        int szMsg;
        MPI_Get_count(&status, Type_MPI<typename K::value_type>::mpi_type(), &szMsg);
        std::vector<typename K::value_type, typename K::allocator_type> *rcv;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            rcv = (std::vector<typename K::value_type, typename K::allocator_type> *)&rcvobj;
            if (rcv->size() < szMsg) {
#if defined(PARALLEL_TRACE)
                log << "Realloc rcv vector to match receive size message" << std::endl;
#endif
                std::vector<typename K::value_type, typename K::allocator_type>(szMsg).swap(*rcv);
            }
        } else {
#if defined(PARALLEL_TRACE)
            log << "Create container data to receive" << std::endl;
#endif
            rcv = new std::vector<typename K::value_type, typename K::allocator_type>(szMsg);
        }
#if defined(PARALLEL_TRACE)
        log << "Receive a container with " << rcv->size() << " elements  from " << sender << " with tag " << tag
            << std::endl;
#endif
        if (Type_MPI<typename K::value_type>::must_be_packed()) {
            MPI_Recv(rcv->data(), rcv->size() * sizeof(typename K::value_type), MPI_BYTE, sender, tag, com, &status);
        } else {
            MPI_Recv(rcv->data(), rcv->size(), Type_MPI<typename K::value_type>::mpi_type(), sender, tag, com, &status);
        }
#if defined(PARALLEL_TRACE)
        log << "OK, receive done !" << std::endl;
#endif
        if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
#if defined(PARALLEL_TRACE)
            log << "Copy temporary vector inside the container passed as "
                   "parameter."
                << std::endl;
#endif
            rcvobj = K(rcv->begin(), rcv->end());
            delete rcv;
        }
        return Status(status);
    }
    // .......................................................................................
    static Request irecv(const MPI_Comm &com, K &rcvobj, int sender, int tag) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
        log << "Asynchronous receive of a container with " << rcvobj.size() << " elements from " << sender
            << " with tag " << tag << std::endl;
#endif
        std::vector<typename K::value_type, typename K::allocator_type> *rcv;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            rcv = (std::vector<typename K::value_type, typename K::allocator_type> *)&rcvobj;
        } else {
#if defined(PARALLEL_TRACE)
            log << "Create container data to receive" << std::endl;
#endif
            rcv = new std::vector<typename K::value_type, typename K::allocator_type>(rcvobj.size());
        }

        MPI_Request req;
        if (Type_MPI<typename K::value_type>::must_be_packed()) {
            MPI_Irecv(rcv->data(), rcv->size() * sizeof(typename K::value_type), MPI_BYTE, sender, tag, com, &req);
        } else {
            MPI_Irecv(rcv->data(), rcv->size(), Type_MPI<typename K::value_type>::mpi_type(), sender, tag, com, &req);
        }

        if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
#if defined(PARALLEL_TRACE)
            log << "Copy temporary vector inside the container passed as "
                   "parameter."
                << std::endl;
#endif
            std::copy(rcvobj.begin(), rcvobj.end(), rcv->begin());
            delete rcv;
        }

        return Request(req);
    }
    // .......................................................................................
    static void broadcast(const MPI_Comm &com, const K *obj_snd, K &obj_rcv, int root) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
#endif
        std::size_t szMsg = (obj_snd != nullptr ? obj_snd->size() : obj_rcv.size());
#if defined(PARALLEL_TRACE)
        log << "Broadcast of a container with " << szMsg << " elements with root = " << root << std::endl;
#endif
        std::vector<typename K::value_type, typename K::allocator_type> *rcv;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            rcv = (std::vector<typename K::value_type, typename K::allocator_type> *)&obj_rcv;
            if (szMsg > rcv->size()) {
#if defined(PARALLEL_TRACE)
                log << "Realloc rcv vector to match broadcast size message" << std::endl;
#endif
                std::vector<typename K::value_type, typename K::allocator_type>(obj_snd->size()).swap(*rcv);
            }
        } else {
#if defined(PARALLEL_TRACE)
            log << "Create container data to receive" << std::endl;
#endif
            rcv = new std::vector<typename K::value_type, typename K::allocator_type>(szMsg);
        }

        int rank;
        MPI_Comm_rank(com, &rank);
        if ((root == rank) && (&obj_rcv != obj_snd)) {
            assert(obj_snd != nullptr);
            std::copy(obj_snd->begin(), obj_snd->end(), obj_rcv.begin());
        }
        if (Type_MPI<typename K::value_type>::must_be_packed()) {
            MPI_Bcast(rcv->data(), rcv->size() * sizeof(typename K::value_type), MPI_BYTE, root, com);
        } else {
            MPI_Bcast(rcv->data(), rcv->size(), Type_MPI<typename K::value_type>::mpi_type(), root, com);
        }
#if defined(PARALLEL_TRACE)
        log << "End of broadcasting" << std::endl;
#endif
        if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
#if defined(PARALLEL_TRACE)
            log << "Copy temporary vector inside the container passed as "
                   "parameter."
                << std::endl;
#endif
            std::copy(obj_rcv.begin(), obj_rcv.end(), rcv->begin());
            delete rcv;
        }
    }
    // .......................................................................................
    static void reduce(const MPI_Comm &com, const K &loc, K *glob, const Operation &op, int root) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
#endif
        std::size_t szMsg = loc.size();
        int rank;
        MPI_Comm_rank(com, &rank);
#if defined(PARALLEL_TRACE)
        log << "Reduce operation on one container with " << szMsg << " elements with root = " << root << std::endl;
#endif
        std::vector<typename K::value_type, typename K::allocator_type> *glb;
        std::vector<typename K::value_type, typename K::allocator_type> *lc;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            glb = (std::vector<typename K::value_type, typename K::allocator_type> *)glob;
            lc  = (std::vector<typename K::value_type, typename K::allocator_type> *)&loc;
            if (glb != nullptr) {
                if (szMsg > glb->size()) {
#if defined(DEBUG)
                    log << "Realloc glob vector to match reduce size "
                           "message"
                        << std::endl;
#endif
                    std::vector<typename K::value_type, typename K::allocator_type>(szMsg).swap(*glb);
                }
            }
        } else {
#if defined(PARALLEL_TRACE)
            log << "Create container data to compute" << std::endl;
#endif
            if (rank == root)
                glb = new std::vector<typename K::value_type, typename K::allocator_type>(szMsg);
            else
                glb = nullptr;
            lc      = new std::vector<typename K::value_type, typename K::allocator_type>(szMsg);
            std::copy(loc.begin(), loc.end(), lc->begin());
        }
        if (glb != nullptr)
            MPI_Reduce(lc->data(), glb->data(), szMsg, Type_MPI<typename K::value_type>::mpi_type(), op, root, com);
        else
            MPI_Reduce(lc->data(), nullptr, szMsg, Type_MPI<typename K::value_type>::mpi_type(), op, root, com);
#if defined(PARALLEL_TRACE)
        log << "End of reduction" << std::endl;
#endif
        if (rank == root)
            if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
                std::copy(glb->begin(), glb->end(), glob->begin());
                delete glb;
                delete lc;
            }
    }
    // ----------------------------------------------------------------------------------------------------
    static void allreduce(const MPI_Comm &com, const K &loc, K *glob, const Operation &op) {
#if defined(PARALLEL_TRACE)
        Core::Logger log;
        log << LogTrace << std::endl;
#endif
        std::size_t szMsg = loc.size();
        int rank;
        MPI_Comm_rank(com, &rank);
#if defined(PARALLEL_TRACE)
        log << Core::Logger::Cyan << "All Reduce operation on one container with " << szMsg << " elements"
            << Core::Logger::Normal << std::endl;
#endif
        std::vector<typename K::value_type, typename K::allocator_type> *glb;
        std::vector<typename K::value_type, typename K::allocator_type> *lc;
        if (std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            glb = (std::vector<typename K::value_type, typename K::allocator_type> *)glob;
            lc  = (std::vector<typename K::value_type, typename K::allocator_type> *)(&loc);
            if (glb != nullptr) {
                if (szMsg > glb->size()) {
#if defined(DEBUG)
                    log << "Realloc glob vector to match reduce size "
                           "message"
                        << std::endl;
#endif
                    std::vector<typename K::value_type, typename K::allocator_type>(szMsg).swap(*glb);
                }
            }
        } else {
#if defined(PARALLEL_TRACE)
            log << "Create container data to compute" << std::endl;
#endif
            glb = new std::vector<typename K::value_type, typename K::allocator_type>(szMsg);
            lc  = new std::vector<typename K::value_type, typename K::allocator_type>(szMsg);
            std::copy(loc.begin(), loc.end(), lc->begin());
        }
        MPI_Allreduce(lc->data(), glb->data(), szMsg, Type_MPI<typename K::value_type>::mpi_type(), op, com);
#if defined(PARALLEL_TRACE)
        log << "End of All reduction" << std::endl;
#endif
        if (!std::is_base_of<std::vector<typename K::value_type, typename K::allocator_type>, K>::value) {
            std::copy(glb->begin(), glb->end(), glob->begin());
            delete glb;
            delete lc;
        }
    }
};
}
#undef BEGIN_PROFILE_COMMUNICATION
#undef END_PROFILE_COMMUNICATION
