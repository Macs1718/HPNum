# include "parallel/log_from_distributed_file.hpp"
# include "parallel/communicator.hpp"
# include <sstream>
# include <iomanip>

namespace Parallel
{
  LogFromDistributedFile::LogFromDistributedFile(int flags,
						 const std::string& basename ) throw(std::ios_base::failure)
  try:
     Core::Logger::Listener(flags),
     m_pt_log(nullptr)
  {
	    Communicator com;
	    std::stringstream file_name;
	    file_name << basename << std::setfill('0') << std::setw(5)
		            << com.rank << ".txt";
      std::unique_ptr<Core::LogToFile> ptr(new Core::LogToFile(flags, file_name.str()));
	    m_pt_log = std::move(ptr);
  }
  catch(std::ios_base::failure) {
    std::cerr << "File creation failed. This listener will be unavailable." << std::endl;
  }
  // ...............................................................................................
  LogFromDistributedFile::LogFromDistributedFile(int flags, const Communicator& com, 
                                                 const std::string& basename ) throw(std::ios_base::failure)
  try:
      Core::Logger::Listener(flags),
      m_pt_log(nullptr)
  {
	    std::stringstream file_name;
	    file_name << basename << std::setfill('0') << std::setw(5)
		            << com.rank << ".txt";
      std::unique_ptr<Core::LogToFile> ptr(new Core::LogToFile(flags, file_name.str()));
	    m_pt_log = std::move(ptr);
  }
  catch(std::ios_base::failure) {
      std::cerr << "File creation failed. This listener will be unavailable." << std::endl;
  }
  // ...............................................................................................
  LogFromDistributedFile::~LogFromDistributedFile()
  {}
};
