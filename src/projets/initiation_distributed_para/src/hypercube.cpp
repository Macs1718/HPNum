#include <iostream>
#include <vector>
#include "core/log_to_std_output.hpp"
#include "core/logger.hpp"
#include "parallel/log_from_distributed_file.hpp"
#include "parallel/log_from_root_output.hpp"
#include "parallel/parallel.hpp"

int main( int nargs, char* args[] ) {
    Parallel::Context context( nargs, args );
    Core::Logger      log;
    log.subscribe( new Parallel::LogFromDistributedFile(
        Core::Logger::Listener::Listen_for_information, "Output" ) );
    log.subscribe( new Parallel::LogFromRootOutput<Core::LogToStdOutput>(
        Core::Logger::Listener::Listen_for_error ) );

    Parallel::Communicator com;

    double x;
    if ( com.rank == 0 ) x = 3.1415;
    // Calcul de la dimension du cube :
    int dim = 0;
    int n   = com.size;
    while ( n > 1 ) {
        dim += 1;
        n >>= 1;
    }
    if ( ( 1 << dim ) != com.size ) {
        log << LogError << "The number of processes must be a power of two !" << std::endl;
    } else {
        for ( int d = 0; d < dim; ++d ) {
            if ( com.rank < ( 1 << d ) )
                com.send( x, com.rank + ( 1 << d ) );
            else if ( com.rank < ( 1 << ( d + 1 ) ) )
                com.recv( x, com.rank - ( 1 << d ) );
        }
        log << LogInformation << "Receive value " << x << std::endl;
    }
    return EXIT_SUCCESS;
}