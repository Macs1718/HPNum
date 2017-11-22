/**
 *     \file    Logger.hpp
 *     \brief   Declaration file for a logger which manages a file or output
 *
 *     \author  Xavier JUVIGNY
 *     \version 0.1
 *     \date    14/03/2017
 */
#ifndef _CORE_LOGGER_HPP_
#define _CORE_LOGGER_HPP_
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>

namespace Core {
    /*!     \class  Logger
     *      \brief  This class manages a  logger which output some categorized
     * messages
     *              following listeners.
     *      \author Dr. Xavier JUVIGNY
     *
     *      A logger is a output system for different levels of messages (
     * Information, Warning, Debug,
     *      Trace, and so on. )
     *      By default, the logger has none listeners.
     *
     *      Exemple :
     *      --------
     *      Core::Logger log;
     *      // Add a listener for the console output for information only
     *      log.subscribe(new
     * Core::LogToStdOutput(Listener::Listen_for_information);
     *      // Add a listener for the error output and assertion :
     *      log.subscribe(new Core::LogFile("ErrorLog",
     * Listener::Listen_for_error |
     *                                                  Listener::Listen_for_assertion
     * );
     *
     *      log_Information << "Bla bla blah" << std::endl;
     *      log_Error       << "Aïe Aïe Aïe"  << std::endl;
     */
    class Logger {
    public:
        /*!    \class  Listener
         *     \brief  Base class for all listeners for the Logger ( as Console,
         * File, Error output )
         *     \author Xavier JUVIGNY
         *
         *     A listener is a realisation of a stream output which writes only
         * some type of messages.
         *     on a specific output. This class below is an abstract class. The
         * concrete classes are
         *     defined in other files.
         *
         */
        class Listener {
        public:
            /*! \enum Listener levels
             *
             *  Describe the level of the listener and the active channels for
             * this listener.
             */
            enum {
                Listen_for_nothing     = 0,
                Listen_for_assertion   = 1,
                Listen_for_error       = 2,
                Listen_for_warning     = 4,
                Listen_for_information = 8,
                Listen_for_trace       = 16,

                Listen_for_all = 0xFFFF
            };

            /*! \brief Construtor  : By default, build a listener who listen
             * none channel.
             */
            Listener( int flags = Listen_for_nothing );
            /*! \brief Destructor : Close the stream associated with the
             * listener.
             */
            virtual ~Listener( );

            /*! \brief Return the ostream associated with the listener.
             */
            virtual std::ostream &report( ) = 0;
            /* ! \brief Test if the listener listen the channel mode given as
             * parameter.
             *
             *   \param mode  The channel mode ( Information, Debug, and so on.
             * )
             */
            bool toReport( int mo ) const { return m_flags & mo; }

        private:
            int m_flags;
        };

        class iterator;
        class const_iterator {
        public:
            const_iterator( const Logger &log, bool end = false );
            const_iterator( const iterator &it );
            const_iterator( const const_iterator &it );
            const_iterator( const_iterator &&it );
            ~const_iterator( );

            const_iterator &operator=( const const_iterator &it );
            const_iterator &operator=( const_iterator &&it );
            bool operator!=( const const_iterator &it ) const;

            const Listener *operator*( ) const;
            const_iterator &operator++( );

        private:
            struct Implementation;
            std::unique_ptr<Implementation> m_pt_impl;
        };

        class iterator {
        public:
            iterator( Logger &log, bool end = false );
            iterator( const iterator &it );
            iterator( iterator &&it );
            ~iterator( );

            iterator &operator=( const iterator &it );
            iterator &operator=( iterator &&it );
            bool operator!=( const iterator &it ) const;

            Listener *operator*( ) const;
            iterator &operator++( );
            friend class const_iterator;

        private:
            struct Implementation;
            std::unique_ptr<Implementation> m_pt_impl;
        };

        /*! \brief Constructor. Build a logger without listener.
             */
        Logger( );

        Logger( const Logger &log ) = delete;
        Logger( Logger &&log )      = delete;
        /*! brief Destructor. Flush all ostream before deleting the logger
         */
        ~Logger( ) { flush( ); }

        Logger &operator=( const Logger & ) = delete;
        Logger &operator=( Logger && ) = delete;

        /*! \brief Add a new listerner for the log
         */
        bool subscribe( Listener *listener );
        /*! \brief Remove a listener from the log.
         */
        bool unsubscribe( Listener *listener );

        /*! \brief Change the channel mode of the messages.
         */
        Logger &operator[]( int mo );

        Logger &set_mode( int mo );
        int get_mode( ) const;

        /*! \brief Flush all ostream contained in the listeners.
         */
        Logger &flush( ) {
            for ( auto listener : *this ) listener->report( ).flush( );
            return *this;
        }
        // ...............................................................................................
        iterator       begin( ) { return iterator( *this, false ); }
        const_iterator begin( ) const { return const_iterator( *this, false ); }
        iterator       end( ) { return iterator( *this, true ); }
        const_iterator end( ) const { return const_iterator( *this, true ); }
        // ..........................................................................
        /*! \brief Flux operator to print an object representation in each
         * listener
         *         who have the current channel mode available.
         */
        template <typename K>
        inline Logger &operator<<( const K &obj ) {
            for ( auto listener : *this )
                if ( listener->toReport( get_mode( ) ) ) { listener->report( ) << obj; }
            return *this;
        }
        /*! \brief Channel modes
         */
        enum {
            nothing     = Listener::Listen_for_nothing,
            assertion   = Listener::Listen_for_assertion,
            error       = Listener::Listen_for_error,
            warning     = Listener::Listen_for_warning,
            information = Listener::Listen_for_information,
            trace       = Listener::Listen_for_trace,
            all         = Listener::Listen_for_all
        };

        /*!
         * Stream class to change the mode of output for the logger
         *
         * Example : logger << Logger::mode(Logger::information) << ...
         */
        struct mode {
        public:
            mode( int n ) : m_param( n ) {}

            int m_param;
        };

        /// \privatesection
        typedef Logger &( *Logger_manip )( Logger & );
        Logger &operator<<( Logger_manip manip ) { return manip( *this ); }

        Logger &operator<<( const mode &m ) {
            set_mode( m.m_param );
            return *this;
        }

        friend class iterator;
        friend class const_iterator;

        // Ansi codes
        static constexpr char Normal[]            = "\033[0m";
        static constexpr char Bright[]            = "\033[1m";
        static constexpr char Underline[]         = "\033[4m";
        static constexpr char Inverse[]           = "\033[7m";
        static constexpr char PrimaryFont[]       = "\033[10m";
        static constexpr char SecondFont[]        = "\033[11m";
        static constexpr char ThirdFont[]         = "\033[12m";
        static constexpr char FourthFont[]        = "\033[13m";
        static constexpr char FifthFont[]         = "\033[14m";
        static constexpr char SixthFont[]         = "\033[15m";
        static constexpr char SeventhFont[]       = "\033[16m";
        static constexpr char HeighthFont[]       = "\033[17m";
        static constexpr char NinthFont[]         = "\033[18m";
        static constexpr char TenthFont[]         = "\033[18m";
        static constexpr char NormalIntensity[]   = "\033[22m";
        static constexpr char NoUnderline[]       = "\033[24m";
        static constexpr char Black[]             = "\033[30m";
        static constexpr char Red[]               = "\033[31m";
        static constexpr char Green[]             = "\033[32m";
        static constexpr char Yellow[]            = "\033[33m";
        static constexpr char Blue[]              = "\033[34m";
        static constexpr char Magenta[]           = "\033[35m";
        static constexpr char Cyan[]              = "\033[36m";
        static constexpr char White[]             = "\033[37m";
        static constexpr char DefaultColor[]      = "\033[49m";
        static constexpr char BBlack[]            = "\033[40m";
        static constexpr char BRed[]              = "\033[41m";
        static constexpr char BGreen[]            = "\033[42m";
        static constexpr char BYellow[]           = "\033[43m";
        static constexpr char BBlue[]             = "\033[44m";
        static constexpr char BMagenta[]          = "\033[45m";
        static constexpr char BCyan[]             = "\033[46m";
        static constexpr char BWhite[]            = "\033[47m";
        static constexpr char DefaultBackground[] = "\033[49m";
        static constexpr char Framed[]            = "\033[51m";
        static constexpr char Encircled[]         = "\033[52m";
        static constexpr char Overlined[]         = "\033[53m";
        static constexpr char NoFramed[]          = "\033[54m";

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m_pt_impl;
    };

//# define LOG Parallel::Context::logger;

#define LogAssert( cond )                                                                                          \
    Core::Logger::mode( ( ( cond ) ? Core::Logger::assertion : Core::Logger::nothing ) )                           \
        << "[\033[33m] [Assertion] " << std::string( __FILE__ ) << " in " << std::string( __FUNCTION__ ) << " at " \
        << std::to_string( __LINE__ ) << " ]\033[0m] : "

#define LogWarning                                                                                                     \
    Core::Logger::mode( Core::Logger::warning ) << "[ \033[31m[Warning] " << std::string( __FILE__ ) << " in "         \
                                                << std::string( __FUNCTION__ ) << " at " << std::to_string( __LINE__ ) \
                                                << " ]\033[0m] :"

#define LogError                                                                                                     \
    Core::Logger::mode( Core::Logger::error ) << "[\033[41;33m [Error] " << std::string( __FILE__ ) << " in "        \
                                              << std::string( __FUNCTION__ ) << " at " << std::to_string( __LINE__ ) \
                                              << " ]\033[0m] : "

#define LogInformation Core::Logger::mode( Core::Logger::information ) << "[\033[32;1mInformation\033[0m] "

#define LogTrace                                                                                                     \
    Core::Logger::mode( Core::Logger::trace ) << "[\033[32m [Trace] " << std::string( __FILE__ ) << " in "           \
                                              << std::string( __FUNCTION__ ) << " at " << std::to_string( __LINE__ ) \
                                              << " ]\033[0m : "
}

namespace std {
    inline Core::Logger &flush( Core::Logger &out ) {
        out.flush( );
        return out;
    }
    inline Core::Logger &endl( Core::Logger &out ) {
        out << "\n";
        out.flush( );
        return out;
    }
}

#endif
