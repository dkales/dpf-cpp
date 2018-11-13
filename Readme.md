===== C++ DPF library ==========


## Executing Microbenchmarks
Example execution of microbenchmarks for a tree with 2^22 elements.

```
make build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
./dpf_pir 22
```
