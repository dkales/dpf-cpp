===== C++ DPF library ==========


## Executing Microbenchmarks
Example execution of microbenchmarks for a tree with 2^22 elements.

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./dpf_pir 22
```
