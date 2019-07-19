cmake_minimum_required(VERSION 3.8)
project(RRpair)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake)

# Guard against in-source builds and bad build-type strings
#
include(ConfigSafeGuards)



#Testing
include(CTest)
#set(LIBS string_dictionaries)

find_package(Threads REQUIRED)
find_package(GTest)
if (GTEST_FOUND)
    set(LIBS ${LIBS} GTest::Main)
else ()

    if (CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-Wall -ansi -Wno-deprecated -pthread)
    endif ()

    if (MSVC)
        #vc 2012 fix for vararg templates
        set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
    endif ()

    include(ConfigGTest)
    set(LIBS ${LIBS} gtest gtest_main)
endif ()



#Benchmark
# If you want to self-test benchmark lib too, turn me ON
#
set(BENCHMARK_ENABLE_TESTING OFF)

# Configure google micro benchmark
#

# c++11 is required
#
if ((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    #    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    #    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

include(ConfigGBench)

#set(CTPH
#        #../bigRePair/bigrepair/ctph/malloc_count.h
#        #../bigRePair/bigrepair/ctph/malloc_count.c
#        ../bigRePair/bigrepair/ctph/newscan.hpp
#        ../bigRePair/bigrepair/ctph/pscan.hpp
#        ../bigRePair/bigrepair/ctph/utils.h
#        ../bigRePair/bigrepair/ctph/utils.c
#        #../bigRePair/bigrepair/ctph/xerrors.h
#        #../bigRePair/bigrepair/ctph/xerrors.c
#        )

set(SOURCE_FILES
        utils/utils.h)


add_executable(RRpair main.cpp ${SOURCE_FILES})



include(internal_utils)


cxx_executable_with_flags(big_repair_build "" "${LIBS};benchmark" tests/big_repair_build.cpp ${SOURCE_FILES})

