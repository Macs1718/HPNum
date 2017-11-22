CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
PROJECT(lifegame VERSION 0.1 LANGUAGES CXX)

####################################################################################
ADD_EXECUTABLE(lifegame 
  src/lifegame.cpp 
  )
TARGET_LINK_LIBRARIES(lifegame gui ${EXTRA_LIBS})
ADD_TEST(lifegame lifegame ${CMAKE_SOURCE_DIR}/projets/lifegame/data/glider_gun.dat)
add_custom_command(TARGET lifegame PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/projets/lifegame/data $<TARGET_FILE_DIR:lifegame)