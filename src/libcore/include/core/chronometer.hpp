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
// Chronometer.hpp
#ifndef _CORE_CHRONOMETER_HPP_
#define _CORE_CHRONOMETER_HPP_
#include <ostream>

namespace Core {
class Chronometer {
  public:
    /**
     * @brief      Default constructor the the chronometer
     */
    Chronometer();
    Chronometer(const Chronometer &chrono) = delete;
    Chronometer(Chronometer &&chrono)      = delete;
    virtual ~Chronometer()                 = default;

    Chronometer &operator=(const Chronometer &) = delete;
    Chronometer &operator=(Chronometer &&) = delete;

    /**
     * @brief      Start the chronometer
     */
    void start();

    /**
     * @brief      Stop the chronometer
     *
     * @return     Return the delta time between start and stop calls
     */
    double stop();

    /**
     * @brief      Compute the mean time between several start and stop
     *             calls
     *
     * @return     Return the mean time
     */
    double mean_time() const;

    /**
     * @brief      Return the total time passed between several calls of
     *             start and stop methods
     *
     * @return     Return the total time
     */
    double total_time() const;

    /**
     * @brief      Return the number of calls for start and stop methods
     *
     * @return     return this number
     */
    unsigned long nb_calls() const;

    /**
     * @brief      Print in the out flux the chronometer measures of time
     *
     * @param      out   The out stream
     *
     * @return     The outstream after written the chronometer measures
     */
    virtual std::ostream &print(std::ostream &out) const;

  protected:
    // start the chronometer
    virtual void start_chrono() = 0;
    // Return the time spended since the start of the chronometer.
    virtual double get_delta_time() = 0;

  private:
    unsigned long m_counter;
    double m_total_time;
    bool m_is_measuring;
};
// ...............................................................................................
/**
 * @brief      Stream flux operator
 *
 * @param      out     The out stream
 * @param[in]  chrono  The chronometer to display on the flux
 *
 * @return     The modified flux operator
 */
inline std::ostream &operator<<(std::ostream &out, const Chronometer &chrono) { return chrono.print(out); }
}

#endif
