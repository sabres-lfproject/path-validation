cmake_minimum_required (VERSION 2.6)
project (PathValidate)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	message(STATUS " ===> Setting default build type RELEASE")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "setting default build type RELEASE" FORCE)
endif()

set(GLOBAL_FLAGS "-Wall -fopenmp -fPIC -std=c++11")
set(CMAKE_CXX_FLAGS "${GLOBAL_FLAGS}")
message(STATUS "Global flags: ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "-O2" CACHE STRING "" FORCE)
message(STATUS "RELEASE flags: ${CMAKE_CXX_FLAGS_RELEASE}")

include_directories("${PROJECT_SOURCE_DIR}/inc")
add_library(zkboopp SHARED src/MpcVariable.cpp src/mpc_types.cpp src/mpc_core.cpp src/zkbpp_core.cpp src/test_utility.cpp)
target_link_libraries(zkboopp crypto)

function(build_executable_cpp cppfile)
  add_executable(${cppfile} app/${cppfile}.cpp)
  target_link_libraries(${cppfile} zkboopp crypto)
endfunction()
function(build_executables_cpp)
  foreach(f ${ARGN})
    build_executable_cpp(${f})
  endforeach()
endfunction()

build_executables_cpp(
    gensecret
    prover
    verifier
    xorHash
)

add_custom_target(
    clean_data
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/data/*.hash
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/data/*.proof
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/data/*.secret
    COMMENT "remove data generating from proofer"
)
