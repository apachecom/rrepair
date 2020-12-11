//
// Created by via56 on 07-12-20.
//

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "../../include/lyndon/lyndon.h"

using namespace lyndon;

auto build = [](benchmark::State & state, const std::string& file)
{
    // Perform setup here
    FILE *pFile;
    long lSize;
    uchar *text;
    size_t result;

    LOG("text file : " << file);
    pFile = fopen(file.c_str(), "rb");
    if (pFile == NULL) {
        ERROR("File error");
        exit(1);
    }
    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    COUT("text size : " << lSize << " [byte]");

    // allocate memory to contain the whole file:
    text = (uchar *)malloc(sizeof(uchar) * (lSize + 1));
    if (text == NULL) {
        ERROR("Memory error");
        exit(2);
    }

    text[0] = 0;
    // copy the file into the buffer:
    result = fread(text + 1, 1, lSize, pFile);
    if (result != lSize) {
        ERROR("Reading error");
        exit(3);
    }
    fclose(pFile);

//    const auto startTime = std::chrono::system_clock::now();
//    LyndonSLP lyndon_slp;
//    lyndon_slp.compress(text, lSize + 1);
//    const auto endTime = std::chrono::system_clock::now();
//    const auto timeSpan = endTime - startTime;
//    uint time = std::chrono::duration_cast<std::chrono::milliseconds>(timeSpan).count();
//    std::cerr << "time: " << time / 1000.0 << "[sec]" << std::endl;



    LyndonSLP lyndon_slp;
    for (auto _ : state) {
        // This code gets timed
        lyndon_slp.compress(text, lSize + 1);

    }

    state.counters["grammar-size"] = lyndon_slp.get_rules().size()*2 + lyndon_slp.get_compressed_text().size();

};




int main (int argc, char *argv[] ){

    std::string coll_file = argv[1];

    benchmark::RegisterBenchmark("lyndon-grammar",build,coll_file)->Unit({benchmark::kMicrosecond});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;

}