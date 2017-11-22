#ifndef _PARALLEL_CHRONOMETER_IMPLEMENTATION_HPP_
#define _PARALLEL_CHRONOMETER_IMPLEMENTATION_HPP_
#include "core/std_cpp_chronometer.hpp"
namespace Parallel {
    struct Communicator::Chronometer::Implementation {
        std::map<std::string, std::unique_ptr<Core::StdChronometer>> m_chronos;
        mutable Core::StdChronometer *pt_current_chronometer;
        bool                          is_activated;
    };
}

#endif