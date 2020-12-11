cmake_minimum_required(VERSION 3.7)
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


set(SOURCE_LIB


#///////////////////C-UTILS///////////////////////////////
#        include/_c/utils.h
#///////////////////RRepair///////////////////////////////
        include/rr/rr_utils.hpp
        include/rr/rr_trie.hpp
        include/rr/rr_sliding_window.hpp
        include/rr/rr_hash_parser.hpp
##///////////////////LC///////////////////////////////
#        include/lc/parser.hpp
#        include/lc/symbols.hpp
#        include/lc/util.hpp
#        include/lc/macros.hpp
##///////////////////GRAMMAR///////////////////////////////
#        include/lc/compressed_grammar.hpp
        include/lc/symbols.hpp
        include/lc/parser.hpp
        include/lc/util.hpp
#        include/indices/bs_index.hpp
#        include/indices/zft_index.hpp
#        include/indices/pt_index.hpp
##///////////////////FINGERPRINTS///////////////////////////////
        include/fingerprints/fingerprints.hpp
        include/fingerprints/algorithms.hpp
##///////////////////LYNDON///////////////////////////////
#        include/lyndon/lyndon.h
##///////////////////LOGGER///////////////////////////////
#        include/logger/CLogger.h
##///////////////////MEM-MONITOR///////////////////////////////
#        include/mem_monitor/mem_monitor.hpp
##///////////////////UTILS///////////////////////////////
        include/utils/aho_aut.hpp
        include/utils/io.hpp
        include/utils/string.hpp
##///////////////////TREES///////////////////////////////
#        include/trees/bp_tree.h
#        include/trees/dfuds_tree.h
#        include/trees/loud_tree.h
#        include/trees/node.h
#        include/trees/trie/compact_trie.h
#        include/trees/trie/Trie.h
##//////////////////INDICE///////////////////////
#        include/indices/interface.hpp
#        include/indices/utils.hpp
##//////////////////GRID///////////////////////
#        include/grid/interface.hpp
#        include/grid/utils.hpp
#
#
##//////////////////G-IMPROVE///////////////////////
#        include/gi/binary_relation.hpp



#///////////////////C-UTILS///////////////////////////////
#        lib/_c/utils.c
#///////////////////RRepair///////////////////////////////
        lib/rr/rr_utils.cpp
        lib/rr/rr_trie.cpp
        lib/rr/rr_sliding_window.cpp
        lib/rr/rr_hash_parser.cpp
##///////////////////LC///////////////////////////////
        lib/lc/parser.cpp
        lib/lc/symbols.cpp
        lib/lc/util.cpp
#        lib/lc/bs_index.cpp
#        lib/lc/zft_index.cpp
#        lib/lc/pt_index.cpp
##///////////////////FINGERPRINTS///////////////////////////////
        lib/fingerprints/fingerprints.cpp
        lib/fingerprints/algorithms.cpp
##///////////////////GRAMMAR///////////////////////////////
#        lib/grammar/compressed_grammar.cpp
##///////////////////LYNDON///////////////////////////////
#        lib/lyndon/lyndon.cpp
##///////////////////LOGGER///////////////////////////////
#        lib/logger/CLogger.cpp
##///////////////////UTILS///////////////////////////////
        lib/utils/aho_aut.cpp
        lib/utils/io.cpp
        lib/utils/string.cpp
##///////////////////TREES///////////////////////////////
#        lib/trees/bp_tree.cpp
#        lib/trees/dfuds_tree.cpp
#        lib/trees/loud_tree.cpp
#        lib/trees/node.cpp
#        lib/trees/trie/compact_trie.cpp
#        lib/trees/trie/Trie.cpp
#
#
##//////////////////G-IMPROVE///////////////////////
#        lib/gi/binary_relation.cpp

        )

set(SOURCE_FILES

        BigRRePair.cpp BigRRePair.h
        DummyPartition.cpp DummyPartition.h
        DummyRepairCompressor.cpp DummyRepairCompressor.h
        utils.h
        model.h
        lib/logger/CLogger.cpp include/logger/CLogger.h
        Partitioner.cpp Partitioner.h

        HashParser.cpp HashParser.h
        SlidingWindow.cpp SlidingWindow.h
        HashFunction.cpp HashFunction.h

        test/UtilStrings.h
        HashParserConfig.cpp HashParserConfig.h
        RepairUtils.h
        RePairRecursive.cpp RePairRecursive.h
        RePairRecursiveConfig.cpp RePairRecursiveConfig.h
        DummyRePair.cpp DummyRePair.h RepairUtils.cpp


        )



#
#
#add_executable(
#        rrpair main.cpp ${SOURCE_FILES}
#)


include(internal_utils)

#cxx_executable_with_flags(t_preprocess_dicc "" "${LIBS};benchmark;sdsl" test/t_preprocess_dicc.cpp ${SOURCE_FILES})
#cxx_executable_with_flags(t_partition "" "${LIBS};benchmark;sdsl" test/t_partition.cpp ${SOURCE_FILES})
#cxx_executable_with_flags(t_recursive_partition "" "${LIBS};benchmark;sdsl" test/t_recursive_partition.cpp ${SOURCE_FILES})
#cxx_executable_with_flags(t_compressor "" "${LIBS};benchmark;sdsl" test/t_compressor.cpp ${SOURCE_FILES})

cxx_executable_with_flags(t_hash_function "" "${LIBS};benchmark" test/t_hash_function.cpp ${SOURCE_FILES} ${SOURCE_LIB})
cxx_executable_with_flags(t_hash_parser "" "${LIBS};sdsl;benchmark" test/t_hash_parser.cpp ${SOURCE_FILES} ${SOURCE_LIB})
cxx_executable_with_flags(t_sliding_window "" "${LIBS};benchmark" test/t_sliding_window.cpp ${SOURCE_FILES})
cxx_executable_with_flags(t_repair_recursive "" "${LIBS};benchmark" test/t_repair_recursive.cpp ${SOURCE_FILES})



cxx_executable_with_flags(rrepair "" "${LIBS};benchmark" bench/rrepair.cpp ${SOURCE_FILES} )

#cxx_executable_with_flags(b_bigrepair_compression "" "${LIBS};benchmark;sdsl" bench/b_bigrepair_compression.cpp ${SOURCE_FILES})
#cxx_executable_with_flags(b_stats "" "${LIBS};benchmark;sdsl" bench/b_stats.cpp ${SOURCE_FILES})
#cxx_executable_with_flags(b_bigrepair_mzz "" "${LIBS};benchmark;sdsl" bench/b_bigrepair_mzz.cpp ${SOURCE_FILES})
#cxx_executable_with_flags(b_repair "" "${LIBS};benchmark;sdsl" bench/b_repair.cpp ${SOURCE_FILES})


cxx_executable_with_flags(t_c_utils "" "${LIBS};benchmark" test/rr/c_utils.cpp ${SOURCE_LIB})
cxx_executable_with_flags(t_utils "" "${LIBS};benchmark" test/rr/utils.cpp ${SOURCE_LIB})
cxx_executable_with_flags(t_trie "" "${LIBS};benchmark" test/rr/trie.cpp ${SOURCE_LIB})
cxx_executable_with_flags(t_fingerprints "" "${LIBS};benchmark" test/rr/fingerprints.cpp ${SOURCE_LIB})
#cxx_executable_with_flags(t_hash_parser "" "${LIBS};benchmark" test/t_hash_parser.cpp ${SOURCE_LIB})

cxx_executable_with_flags(t_lyndon "" "${LIBS};sdsl;benchmark" test/lyndon/build.cpp ${SOURCE_LIB})


cxx_executable_with_flags(t_lc_parser "" "${LIBS};sdsl;benchmark" test/lc/parser.cpp ${SOURCE_LIB})




#target_link_libraries(rrpair sdsl)
