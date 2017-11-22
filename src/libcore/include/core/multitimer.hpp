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
#ifndef _CORE_MULTITIMER_HPP_
#define _CORE_MULTITIMER_HPP_
#include <memory>
#include <string>

namespace Core {
    class Chronometer;
    /**
     * brief      Container for a dictionnary of timer
     *
     * @tparam     Key   The type of the key for the dictionnary ( string by default )
     */
    template <typename Key = std::string> class MultiTimer {
    public:
        class iterator;
        /**
         * @brief      Constant iterator through the chronometers of the multitimer instance
         * 
         */
        class const_iterator {
        public:
            const_iterator( const MultiTimer &log, bool end = false );
            const_iterator( const iterator &it );
            const_iterator( const const_iterator &it );
            const_iterator( const_iterator &&it );
            ~const_iterator( );

            const_iterator &operator=( const iterator &it );
            const_iterator &operator=( const const_iterator &it );
            const_iterator &operator=( const_iterator &&it );

            const Chronometer &operator*( ) const;
            bool operator!=( const const_iterator &it ) const;
            const Key &getKey( ) const;
            const_iterator &operator++( );

        private:
            struct Implementation;
            std::unique_ptr<Implementation> m_pt_impl;
        };
        // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        /**
         * @brief      Iterator through the chronometers of the multitimer instance
         * 
         */
        class iterator {
        public:
            iterator( MultiTimer &log, bool end = false );
            iterator( const iterator &it );
            iterator( iterator &&it );
            ~iterator( );

            iterator &operator=( const iterator &it );
            iterator &operator=( iterator &&it );

            Chronometer &operator*( ) const;
            bool operator!=( const iterator &it ) const;
            const Key &getKey( ) const;
            iterator &operator++( );
            friend const_iterator;

        private:
            struct Implementation;
            std::unique_ptr<Implementation> m_pt_impl;
        };
        // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        /**
         * @brief Void multitimer instance
         */
        MultiTimer( );
        MultiTimer( const MultiTimer & ) = delete;
        MultiTimer( MultiTimer && )      = delete;
        /**
         * @brief      Destroys the multitimer instance
         */
        ~MultiTimer( );

        /**
         * @brief      Subscribe a new chronometer 
         *
         * @param[in]  label      The label to identify the chronometer
         * @param[in]  ... args   Arguments ( can be void ) to build the chronometer     
         *
         * @tparam     Chrono     The type of the chronometer
         * @tparam     Args       The list of the type of the arguments
         *
         * @return     A new chronometer instance.
         */
        template <class Chrono, class... Args>
        Chronometer &subscribe( const Key &label, Args &&... args ) {
            std::unique_ptr<Chronometer> t(
                new Chrono( std::forward<Args>( args )... ) );
            register_as( t, label );
            return *t;
        }
        /**
         * @brief      Remove a chronometer of the multitimer
         *
         * @param[in]  label  The identificator of the chronometer
         */
        void unsubscribe( const Key &label );

        /**
         * @brief      Return a reference on the chronometer identified by label
         *
         * @param[in]  label  The label identificator
         *
         * @return     Return the reference of the chronometer
         */
        Chronometer &operator[]( const Key &label );
        /**
         * @brief      Return a const reference on the chronometer identified by label
         *
         * @param[in]  label  The label identificator
         *
         * @return     Return the const reference of the chronometer
         */
        const Chronometer &operator[]( const Key &label ) const;

        /**
         * @brief      Display the chronometers statistics on the output stream
         *
         * @param      out   The output stream
         *
         * @return     The modified output stream
         */
        std::ostream &print( std::ostream &out ) const;

        /**
         * @brief      Return an iterator on the first chronometer of the multitimer instance
         *
         * @return     An iterator
         */
        iterator       begin( ) { return iterator( *this, false ); }
        /**
         * @brief      Return a const_iterator on the first chronometer of the multitimer instance
         *
         * @return     A const_iterator
         */
        const_iterator begin( ) const { return const_iterator( *this, false ); }
        /**
         * @brief      Return an iterator after the last chronometer of the multitimer instance
         *
         * @return     An iterator
         */
        iterator       end( ) { return iterator( *this, true ); }
        /**
         * @brief      Return a const_iterator after the last chronometer of the multitimer instance
         *
         * @return     A const_iterator
         */
        const_iterator end( ) const { return const_iterator( *this, true ); }

        friend iterator;
        friend const_iterator;

    private:
        void register_as( std::unique_ptr<Chronometer> &chrono,
                          const Key &                   label );

        struct Implementation;
        std::shared_ptr<Implementation> m_pt_impl;
    };
    // ..........................................................................................

    /*
     * brief       The stream operator to display the chronometers in the multitimer instance
     */    
    template <typename Key>
    inline std::ostream &operator<<( std::ostream &         out,
                                     const MultiTimer<Key> &log ) {
        return log.print( out );
    }
}

#endif
