#!/bin/bash
echo building repair integer version
cd external/repair
make
cd ../../

mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
echo building tests

make t_hash_function
make t_sliding_windows
make t_hash_parser
make t_recursive_repair


./t_hash_function
./t_sliding_windows
./t_hash_parser
./t_recursive_repair

echo building binnary

make rrepair
./rrepair
