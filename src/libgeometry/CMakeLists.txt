CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(libgeometry LANGUAGES CXX)

ADD_LIBRARY(geometry SHARED
  src/box.cpp
  src/boundingbox.cpp
  src/kdtree.cpp)

TARGET_INCLUDE_DIRECTORIES(geometry PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

TARGET_COMPILE_FEATURES(geometry
  PUBLIC cxx_auto_type
  PRIVATE cxx_variadic_templates)

#TARGET_LINK_LIBRARIES(core (librairie externe à linker))
INSTALL(TARGETS geometry EXPORT geometryconfig
  ARCHIVE  DESTINATION lib
  LIBRARY  DESTINATION lib
  RUNTIME  DESTINATION bin)  # This is for Windows

INSTALL(DIRECTORY include/ DESTINATION include)

INSTALL(EXPORT geometryconfig DESTINATION share/geometry/cmake)

EXPORT(TARGETS geometry FILE geometryconfig.cmake)

ADD_EXECUTABLE(test_box test/test_box.cpp)
TARGET_LINK_LIBRARIES(test_box geometry core)
ADD_TEST(test_box test_box)

ADD_EXECUTABLE(test_boundingbox test/test_boundingbox.cpp)
TARGET_LINK_LIBRARIES(test_boundingbox geometry core)
ADD_TEST(test_boundingbox test_boundingbox)
