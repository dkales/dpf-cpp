# C++ DPF-PIR library

This is a high-performance implementation of private information retrieval (PIR) 
based on distributed point functions (DPF). As this is a multi-server 
(more specifically a 2-server) PIR, we need two *non-colluding* servers 
with identical databases. For our use case, where this DPF-PIR is being used in a
Certificate Transparency (CT) log server, we specialize on databases of SHA-256 hash
values, which are stored in a *hashdatastore* object. This aligns nicely with the use
of AVX/AVX2 instructions to speed up calculations. We also make use of AES-NI instructions
to create a high-performance PRF.


## Executing Microbenchmarks
Example execution of microbenchmarks for a tree with 2^22 elements.

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./dpf_pir 22
```

## References

Some parts like the AES-NI implementation are taken from Peter Rindal's public domain [CryptoTools](https://github.com/ladnir/cryptoTools/).

[Paper](http://www.ramacher.at/_static/papers/ct-privacy.pdf): *Daniel Kales, Olamide Omolola, Sebastian Ramacher*. **Revisting User Privacy for Certificate Transparency**. EuroS&P 2019
