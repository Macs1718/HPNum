#include "core/multitimer.hpp"
#include "core/multitimer.tpp"
#include <string>

namespace Core {
    template class MultiTimer<std::string>;
    template class MultiTimer<int>;
};
