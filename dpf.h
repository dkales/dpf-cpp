#pragma once

#include <cstdlib>
#include <cstdint>
#include <vector>

#include <droidCrypto/BitVector.h>
using droidCrypto::BitVector;

namespace dpf {
    class DPF {
    public:
        static std::pair<std::vector<uint8_t>, std::vector<uint8_t> > Gen(size_t alpha, size_t logn);
        static bool Eval(const std::vector<uint8_t>& key, size_t x, size_t logn);
        static BitVector EvalFull(const std::vector<uint8_t>& key, size_t logn);
    };
}
