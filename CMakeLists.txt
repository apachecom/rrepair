cmake_minimum_required(VERSION 3.13)
project(rrpair)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake)

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


set(SOURCE_FILES

        BigRRePair.cpp BigRRePair.h
        DummyPartition.cpp DummyPartition.h
        DummyRepairCompressor.cpp DummyRepairCompressor.h
        utils.h
        model.h
        CLogger.cpp CLogger.h
        Partitioner.cpp Partitioner.h

        HashParser.cpp HashParser.h
        SlidingWindow.cpp SlidingWindow.h
        HashFunction.cpp HashFunction.h

        test/UtilStrings.h
        HashParserConfig.cpp HashParserConfig.h
        RepairUtils.h
        RePairRecursive.cpp RePairRecursive.h
        RePairRecursiveConfig.cpp RePairRecursiveConfig.h
        DummyRePair.cpp DummyRePair.h RepairUtils.cpp bench/rrepair.cpp)





add_executable(
        rrpair main.cpp ${SOURCE_FILES}
)


include(internal_utils)

cxx_executable_with_flags(t_preprocess_dicc "" "${LIBS};benchmark;sdsl" test/t_preprocess_dicc.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_partition "" "${LIBS};benchmark;sdsl" test/t_partition.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_recursive_partition "" "${LIBS};benchmark;sdsl" test/t_recursive_partition.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_compressor "" "${LIBS};benchmark;sdsl" test/t_compressor.cpp ${SOURCE_FILES})

cxx_executable_with_flags(t_hash_function "" "${LIBS};benchmark;sdsl" test/t_hash_function.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_hash_parser "" "${LIBS};benchmark;sdsl" test/t_hash_parser.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_sliding_window "" "${LIBS};benchmark;sdsl" test/t_sliding_window.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_repair_recursive "" "${LIBS};benchmark;sdsl" test/t_repair_recursive.cpp ${SOURCE_FILES})



cxx_executable_with_flags(rrepair "" "${LIBS};benchmark;sdsl" bench/rrepair.cpp ${SOURCE_FILES})

cxx_executable_with_flags(b_bigrepair_compression "" "${LIBS};benchmark;sdsl" bench/b_bigrepair_compression.cpp ${SOURCE_FILES})
cxx_executable_with_flags(b_stats "" "${LIBS};benchmark;sdsl" bench/b_stats.cpp ${SOURCE_FILES})
cxx_executable_with_flags(b_bigrepair_mzz "" "${LIBS};benchmark;sdsl" bench/b_bigrepair_mzz.cpp ${SOURCE_FILES})
cxx_executable_with_flags(b_repair "" "${LIBS};benchmark;sdsl" bench/b_repair.cpp ${SOURCE_FILES})



target_link_libraries(rrpair sdsl)
