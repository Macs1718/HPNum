CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(initiation_distributed_para LANGUAGES CXX)

####################################################################################
ADD_EXECUTABLE(parallel_block_matrix.exe
  parallel_block_matrix.cpp 
  )
TARGET_LINK_LIBRARIES(parallel_block_matrix.exe parallel ${EXTRA_LIBS})
ADD_TEST(NAME parallel_matrix_debug COMMAND parallel_block_matrix.exe 12 trace)
ADD_TEST(NAME parallel_matrix_perfo COMMAND parallel_block_matrix.exe 600 profile)
