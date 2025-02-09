cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

set(ENABLE_LTO True)
project(Assignment1)
set(CMAKE_VERBOSE_MAKEFILE True)
find_package(PkgConfig)
find_package(Boost 1.86.0)
#set(ENABLE_OPENMP False)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -Wextra -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -L$ENV{HOME}/sysroot/lib/")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${Boost_INCLUDE_DIRS}")
set(CMAKE_CXX_COMPILER "g++")
if(ENABLE_OPENMP)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()
if(ENABLE_MPI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIS_MPI")
    set(CMAKE_CXX_COMPILER "mpicxx")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

#Release = rel with debug info. Just strip the debug info out if you don't want it.
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -ffast-math")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELEASE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# We always want our asserts checked.
remove_definitions("-DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(ENABLE_PROF AND ENABLE_COV)
  message(FATAL_ERROR "Cannot enable profiling and code coverage")
endif()

if(ENABLE_PROF)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fprofile-arcs -ftest-coverage")
  #set(CMAKE_BUILD_TYPE Release)
  # LTO with profile builds causes functions to not be emitted
  set(ENABLE_LTO False)
  set(CMAKE_CXX_FLAGS_DEBUG "-Og")
endif()

# Enable LTO for better performance.
if(ENABLE_LTO STREQUAL True AND (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
  check_ipo_supported(RESULT supported OUTPUT error)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

pkg_check_modules(GSL REQUIRED gsl)
add_executable(main src/main.cc src/camera.cc src/interval.cc src/solve.cc src/decomp.cc src/img.cc src/img_data.cc)
add_executable(tgsl src/gsl.cc src/solve.cc)
#target_link_libraries(main gslcblas gsl)
target_link_libraries(main ${GSL_LIBRARIES})
target_include_directories(main PUBLIC ${GSL_INCLUDE_DIRS})
target_compile_options(main PUBLIC ${GSL_CFLAGS_OTHER})

target_link_libraries(tgsl ${GSL_LIBRARIES})
target_include_directories(tgsl PUBLIC ${GSL_INCLUDE_DIRS})
target_compile_options(tgsl PUBLIC ${GSL_CFLAGS_OTHER})
#target_link_libraries(tgsl gslcblas)
#target_link_libraries(tgsl gsl)
target_link_libraries(main jpeg)
#target_link_libraries(main boost_gil_jpeg_extension)

if(ENABLE_COV)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set(CMAKE_BUILD_TYPE Debug)
  target_link_libraries(test gcov)
endif()
