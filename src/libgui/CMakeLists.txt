CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(libgui LANGUAGES CXX)

####################################################################################
ADD_LIBRARY(gui SHARED
  src/window.cpp
  src/context.cpp
  src/event_manager.cpp
  )
TARGET_LINK_LIBRARIES(gui ${EXTRA_LIBS})

TARGET_INCLUDE_DIRECTORIES(gui PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

TARGET_COMPILE_FEATURES(gui
  PUBLIC cxx_auto_type
  PUBLIC cxx_return_type_deduction
  PRIVATE cxx_variadic_templates
  PRIVATE cxx_variable_templates)

#TARGET_LINK_LIBRARIES(core (librairie externe à linker))
INSTALL(TARGETS gui EXPORT guiconfig
  ARCHIVE  DESTINATION lib
  LIBRARY  DESTINATION lib
  RUNTIME  DESTINATION bin)  # This is for Windows

INSTALL(DIRECTORY include/ DESTINATION include)

INSTALL(EXPORT guiconfig DESTINATION share/gui/cmake)

EXPORT(TARGETS gui FILE guiConfig.cmake)

ADD_EXECUTABLE(test_rendering test/test_rendering.cpp)
TARGET_LINK_LIBRARIES(test_rendering gui core)
ADD_TEST(test_rendering test_rendering)

#ADD_EXECUTABLE(test_parallelmatrixmatrixproduct test/test_parallelmatrixmatrixproduct.cpp)
#TARGET_LINK_LIBRARIES(test_parallelmatrixmatrixproduct parallel core)
#ADD_TEST(test_parallelmatrixmatrixproduct test_parallelmatrixmatrixproduct)
