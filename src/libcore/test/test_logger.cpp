#include "core/logger"
using namespace Core;

void g() { Logger() << LogInformation << "Calling g. . ." << std::endl; }

void f() { Logger() << LogTrace << "Calling f..." << std::endl; }

int main() {
    Logger log;
    log.subscribe(new LogToStdOutput(Logger::Listener::Listen_for_information));
    log.subscribe(new LogToStdError(Logger::Listener::Listen_for_assertion | Logger::Listener::Listen_for_error |
                                    Logger::Listener::Listen_for_warning));
    Logger::Listener *file_listener = new LogToFile(Logger::Listener::Listen_for_trace, "Trace.txt");
    log.subscribe(file_listener);

    log << LogInformation << "Information from main " << std::endl;
    log << LogAssert(true) << " is displaying" << std::endl;
    log << LogAssert(false) << " may not displayed" << std::endl;
    log << LogWarning << "Warning test" << std::endl;
    log << LogError << "Fictionnal error" << std::endl;

    f();    
    g();
    log << "Continuing with same mode than g" << std::endl;
    log.unsubscribe(file_listener);
    f(); // Doesn't write on the file...
    log << Logger::mode(Logger::information) << "Change message mode --> Information" << std::endl;
    log << "information mode too" << std::endl;
    delete file_listener;
    return EXIT_SUCCESS;
}
