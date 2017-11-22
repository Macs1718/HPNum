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
    log.subscribe( new Parallel::LogFromDistributedFile(
        Core::Logger::Listener::Listen_for_information, "Output" ) );
    Parallel::Communicator com;

    std::vector<int> rake(com.size), rake2(com.size);
    for ( int i = 0; i < rake.size(); ++i ) rake[i] = (com.rank+0.5)*(i+1.);
        
    int sender = (com.rank+com.size-1)%com.size;
    int receiver = (com.rank+1)%com.size;
    auto req1 = com.irecv(rake2, sender);
    auto req2 = com.isend(rake , receiver);
    req1.wait();
    req2.wait();
    log << LogInformation << "Final rake in master proc : ";
    for ( const auto& v : rake2 ) log << v << " ";
    log << std::endl;
    return EXIT_SUCCESS;
}