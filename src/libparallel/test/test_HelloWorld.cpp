#include "core/log_to_std_output.hpp"
#include "core/logger.hpp"
#include "parallel/communicator"
#include "parallel/context.hpp"
#include "parallel/log_from_distributed_file.hpp"
#include "parallel/log_from_root_output.hpp"
#include <iostream>

int main( int argc, char *argv[] ) {
    Parallel::Context context( argc, argv );
    Core::Logger      log;
    log.subscribe( new Parallel::LogFromRootOutput<Core::LogToStdOutput>(
        Core::Logger::Listener::Listen_for_warning ) );
    log.subscribe( new Parallel::LogFromDistributedFile(
        Core::Logger::Listener::Listen_for_information, "Output" ) );

    Parallel::Communicator com;

    log << LogWarning << "This message must be displayed one time only !"
        << std::endl;
    log << LogInformation << "Hello World from " << com.rank << std::endl;

    return EXIT_SUCCESS;
}
