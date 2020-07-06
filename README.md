# rrepair
Recursive version of the Re-Pair algorithm for large repetitive collections.

## Clone the repo
```
git clone https://github.com/apachecom/rrepair.git
```

## Execute config file 
```
./config.sh
```
## Use the binary cmake-build-release/rrepair to run the compressor
```
usage is <filename> <ws> <mod> <max-iter> <th-len-seq>
```
where:

- filename: file to compress 
- ws:  windows size used by the parser
- mod: module value used by the parser
- max-iter: maximum iterations in the recursive partition
- th-len-seq: the threshold for the length of the initial sequence of the parser 
