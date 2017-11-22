# include <iostream>
# include "parallel/parallel.hpp"

int main(int argc, char* argv[] )
{
    Parallel::Context context( argc, argv );
    Parallel::Communicator com; // global communicator if none arguments

    std::cout << "Hello from " << com.rank << " on " << com.size << std::endl;
    return EXIT_SUCCESS;
}