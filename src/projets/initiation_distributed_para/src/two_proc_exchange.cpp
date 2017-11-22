# include "parallel/parallel.hpp"
# include "core/logger.hpp"
# include "core/log_to_std_output.hpp"
# include "parallel/log_from_distributed_file.hpp"
# include "parallel/log_from_root_output.hpp"
# include <iostream>

int main( int nargs, char* args[] )
{
    Parallel::Context context( nargs, args );
    Core::Logger      log;
    log.subscribe( new Parallel::LogFromRootOutput<Core::LogToStdOutput>(
        Core::Logger::Listener::Listen_for_warning ) );
    log.subscribe( new Parallel::LogFromDistributedFile(
        Core::Logger::Listener::Listen_for_information, "Output" ) );
    
    Parallel::Communicator com;

    int token = 10 * com.rank + 5;
    if ( com.rank == 0 )
    {
        log << LogInformation << "Sending " << token << " to processus number one !" << std::endl;
        com.send( token, 1 );
        com.recv( token, 1 );
        log << LogInformation << "Received " << token << " from processus number one !" << std::endl;
    } else {
        log << LogInformation << "Sending " << token << " to processus number zero !" << std::endl;
        com.send( token, 0);
        com.recv( token, 0);
        log << LogInformation << "Received " << token << " from processus number zero !" << std::endl;
    }
    return EXIT_SUCCESS;
}