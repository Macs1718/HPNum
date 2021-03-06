CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
PROJECT(liblinearalgebra LANGUAGES CXX)

###### SEARCH THE BLAS AND LAPACK PACKAGES #######
SET(BLAS_DIR "/usr/lib" CACHE STRING "Path where find good version of the blas library")
FIND_PACKAGE(BLAS REQUIRED)
SET (EXTRA_LIBS ${EXTRA_LIBS} ${BLAS_LIBRARIES})
SET (EXTRA_LINK_FLAGS ${EXTRA_LINK_FLAGS} ${BLAS_LINKER_FLAGS})
FIND_PATH(BLAS_INCLUDE_DIRS *cblas.h
  /usr/include
  /usr/include/atlas
  /usr/include/openblas
  /usr/local/include
  $ENV{BLAS_HOME}/include)

SET(LAPACK_DIR "/usr/lib" CACHE STRING "Path where find good version of the Lapack library")
FIND_PACKAGE(LAPACK REQUIRED)
SET (EXTRA_LIBS ${EXTRA_LIBS} ${LAPACK_LIBRARIES})
SET (EXTRA_LIBS ${EXTRA_LIBS} cblas)
SET (EXTRA_LINK_FLAGS ${EXTRA_LINK_FLAGS} ${LAPACK_LINKER_FLAGS})
FIND_PATH(LAPACK_INCLUDE_DIRS *lapacke.h
  /usr/include
  /usr/include/atlas
  /usr/include/openblas
  /usr/local/include
  $ENV{LAPACK_HOME}/include)

FIND_PACKAGE(LapackE REQUIRED)
SET (EXTRA_LIBS ${EXTRA_LIBS} ${LAPACKE_LIBRARIES})
INCLUDE_DIRECTORIES(${LAPACKE_INCLUDE_DIR})

ADD_LIBRARY(linearalgebra SHARED
    src/blas2ext.cpp
    )
TARGET_LINK_LIBRARIES(linearalgebra ${EXTRA_LIBS})

TARGET_INCLUDE_DIRECTORIES(linearalgebra PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

TARGET_COMPILE_FEATURES(linearalgebra
  PUBLIC cxx_auto_type
  PRIVATE cxx_variadic_templates)

#TARGET_LINK_LIBRARIES(linearalgebra (librairie externe à linker))
INSTALL(TARGETS linearalgebra EXPORT linearalgebraconfig
  ARCHIVE  DESTINATION lib
  LIBRARY  DESTINATION lib
  RUNTIME  DESTINATION bin)  # This is for Windows

INSTALL(DIRECTORY include/ DESTINATION include)

INSTALL(EXPORT linearalgebraconfig DESTINATION share/linearalgebra/cmake)

EXPORT(TARGETS linearalgebra FILE linearalgebraconfig.cmake)
