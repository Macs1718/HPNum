# include "parallel/parallel.hpp"
# include "core/logger.hpp"
# include "core/log_to_std_output.hpp"
# include "parallel/log_from_distributed_file.hpp"
# include "parallel/log_from_root_output.hpp"
# include <iostream>
# include <vector>

int main( int nargs, char* args[] )
{
    Parallel::Context context(nargs, args);
    Core::Logger      log;
    log.subscribe( new Parallel::LogFromRootOutput<Core::LogToStdOutput>(
                             Core::Logger::Listener::Listen_for_information ) );
    Parallel::Communicator com;

    std::vector<int> rake(com.size);
    if ( com.rank == 0 ) {
        rake[com.rank] = com.rank*101;
        com.send(rake, 1);
        com.recv(rake, com.size-1);
        log << LogInformation << "Final rake in master proc : ";
        for ( const auto& v : rake ) log << v << " ";
        log << std::endl;
    } else {
        com.recv(rake, com.rank-1);
        rake[com.rank] = com.rank*101;
        com.send(rake, (com.rank+1)%com.size);
    }
    return EXIT_SUCCESS;
}