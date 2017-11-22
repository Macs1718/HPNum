CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(initiation_distributed_para LANGUAGES CXX)

####################################################################################
ADD_EXECUTABLE(HelloWorld.exe
  src/HelloWorld.cpp 
  )
TARGET_LINK_LIBRARIES(HelloWorld.exe parallel ${EXTRA_LIBS})
ADD_TEST(HelloWorld.exe HelloWorld.exe)

ADD_EXECUTABLE(HelloWorld_to_log.exe
  src/HelloWorld_to_log.cpp 
  )
TARGET_LINK_LIBRARIES(HelloWorld_to_log.exe parallel core ${EXTRA_LIBS})
ADD_TEST(HelloWorld_to_log.exe HelloWorld_to_log.exe)

ADD_EXECUTABLE(two_proc_exchange.exe
  src/two_proc_exchange.cpp 
  )
TARGET_LINK_LIBRARIES(two_proc_exchange.exe parallel core ${EXTRA_LIBS})
ADD_TEST(two_proc_exchange.exe two_proc_exchange.exe)

ADD_EXECUTABLE(token_ring_synchronous.exe
  src/token_ring_synchronous.cpp 
  )
TARGET_LINK_LIBRARIES(token_ring_synchronous.exe parallel core ${EXTRA_LIBS})
ADD_TEST(token_ring_synchronous.exe token_ring_synchronous.exe)

ADD_EXECUTABLE(token_ring_asynchronous.exe
  src/token_ring_asynchronous.cpp 
  )
TARGET_LINK_LIBRARIES(token_ring_asynchronous.exe parallel core ${EXTRA_LIBS})
ADD_TEST(token_ring_asynchronous.exe token_ring_asynchronous.exe)

ADD_EXECUTABLE(hypercube.exe
  src/hypercube.cpp 
  )
TARGET_LINK_LIBRARIES(hypercube.exe parallel core ${EXTRA_LIBS})
ADD_TEST(hypercube.exe hypercube.exe)
