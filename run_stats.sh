#!/bin/bash

cd cmake-build-release
make
./b_bigrepair_mzz  ../dir_coll --benchmark_out=../b_bigrepair_mzz.csv  --benchmark_out_format=csv --benchmark_counters_tabular=true
./b_stats          ../dir_coll --benchmark_out=../b_stats2.csv          --benchmark_out_format=csv --benchmark_counters_tabular=true
