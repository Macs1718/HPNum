CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(libparallel LANGUAGES CXX)

####################################################################################
OPTION (PARALLEL_TRACE "Trace the parallel functions call if on." OFF)
IF (PARALLEL_TRACE)
  ADD_DEFINITIONS( -DPARALLEL_TRACE=1 )
ENDIF (PARALLEL_TRACE)
####################################################################################
ADD_LIBRARY(parallel SHARED
  src/context_mpi.cpp
  src/context_stub.cpp
  src/log_from_distributed_file.cpp
  src/communicator.cpp
  )
TARGET_LINK_LIBRARIES(parallel core ${EXTRA_LIBS})

TARGET_INCLUDE_DIRECTORIES(parallel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

TARGET_COMPILE_FEATURES(parallel
  PUBLIC cxx_auto_type
  PUBLIC cxx_return_type_deduction
  PRIVATE cxx_variadic_templates
  PRIVATE cxx_variable_templates)

#TARGET_LINK_LIBRARIES(core (librairie externe à linker))
INSTALL(TARGETS parallel EXPORT parallelconfig
  ARCHIVE  DESTINATION lib
  LIBRARY  DESTINATION lib
  RUNTIME  DESTINATION bin)  # This is for Windows

INSTALL(DIRECTORY include/ DESTINATION include)

INSTALL(EXPORT parallelconfig DESTINATION share/parallel/cmake)

EXPORT(TARGETS parallel FILE parallelConfig.cmake)

ADD_EXECUTABLE(test_HelloWorld test/test_HelloWorld.cpp)
TARGET_LINK_LIBRARIES(test_HelloWorld parallel core)
ADD_TEST(test_HelloWorld test_HelloWorld)

ADD_EXECUTABLE(test_parallelmatrixmatrixproduct test/test_parallelmatrixmatrixproduct.cpp)
TARGET_LINK_LIBRARIES(test_parallelmatrixmatrixproduct parallel core)
ADD_TEST(test_parallelmatrixmatrixproduct test_parallelmatrixmatrixproduct)
