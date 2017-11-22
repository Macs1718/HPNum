#include "core/multitimer.hpp"
#include "core/std_cpp_chronometer.hpp"
#include <cmath>
#include <iostream>
#include <list>

/**
 * @brief      Compute the syracuse serie :
 *             u_{n+1} = u_{n}/2 if u_{n} is even
 *                     = 3 * u_{n} + 1 if u_{n} is odd.
 *
 * @param[in]  u0    The initial u_{n}
 *
 * @return     Number of iteration to reach one.
 */
int syracuse_len_flight( int u0 ) {
    int len_flight = 0;
    while ( u0 != 1 ) {
        u0 = ( u0 & 1 ? 3 * u0 + 1 : u0 / 2 );
        len_flight += 1;
    }
    return len_flight;
}

/**
 * @brief      Prime decomposition of an integer
 *
 * @param[in]  n     The integer to decompose
 *
 * @return     The list of prime factor which divide the number n
 */
std::list<unsigned long> decomp( unsigned long n ) {
    std::list<unsigned long> primes;
    while ( n % 2 == 0 ) {
        primes.push_back( 2UL );
        n /= 2;
    }
    unsigned long prime = 3;
    while ( prime <= std::sqrt( n ) ) {
        while ( n % prime == 0 ) {
            primes.push_back( prime );
            n /= prime;
        }
        prime += 2;
    }
    primes.push_back( n );
    return primes;
}

int main( ) {
    const int syracuse_id = 0;
    const int decomp_id   = 2;

    Core::MultiTimer<int>         timer;
    Core::MultiTimer<std::string> stimer;
    timer.subscribe<Core::StdChronometer>( syracuse_id );
    timer.subscribe<Core::StdChronometer>( decomp_id );
    stimer.subscribe<Core::StdChronometer>( "Syracuse" );
    stimer.subscribe<Core::StdChronometer>( "P-adic decomposition" );

    stimer["Syracuse"].start( );
    timer[syracuse_id].start( );
    int max_flght = 0;
    for ( int u0 = 3; u0 < 100000; u0 += 2 ) {
        int len_flght = syracuse_len_flight( u0 );
        max_flght     = ( max_flght > len_flght ? max_flght : len_flght );
    }
    timer[syracuse_id].stop( );
    stimer["Syracuse"].stop( );
    std::cout << "Maximal length for flight : " << max_flght << std::endl;

    stimer["P-adic decomposition"].start( );
    timer[decomp_id].start( );
    unsigned long n   = 624214558968UL;
    auto          dec = decomp( n );
    timer[decomp_id].stop( );
    stimer["P-adic decomposition"].stop( );
    std::cout << "Primer number decomposition of " << n << " is : ";
    for ( const auto &val : dec )
        std::cout << val << " x ";
    std::cout << "1" << std::endl;

    std::cout << "Time passed in functions : ( 0 = syracuse, 2 = p-adic "
                 "decomposition)\n";
    std::cout << timer << std::endl;
    std::cout << stimer << std::endl;
    return EXIT_SUCCESS;
}
