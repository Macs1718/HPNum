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
// std_chronometer.hpp
#ifndef _CORE_STD_CHRONOMETER_HPP_
#define _CORE_STD_CHRONOMETER_HPP_
#include "core/chronometer.hpp"
#include <chrono>
#include <ctime>

namespace Core {
class StdChronometer : public Chronometer {
  public:
    /** @brief Default constructor
     *
     *  Initialize the standard library chronometer
     */
    StdChronometer()                             = default;
    StdChronometer(const StdChronometer &chrono) = delete;
    StdChronometer(StdChronometer &&chrono)      = delete;
    /** @brief       Destructor
     *
     * Free the standard chronometer.
     */
    virtual ~StdChronometer() = default;

    StdChronometer &operator=(const StdChronometer &) = delete;
    StdChronometer &operator=(StdChronometer &&) = delete;

  protected:
    // Démarre le chronomètre
    virtual void start_chrono() final;
    // Renvoie le temps en seconde écoulé depuis l'appel de start_chrono.
    virtual double get_delta_time() override;

  private:
    std::chrono::time_point<std::chrono::system_clock> m_start;
};
}
#endif
