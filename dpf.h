#pragma once

#include <cstdlib>
#include <cstdint>
#include <vector>

#include "BitVector.h"

namespace DPF {
    std::pair<std::vector<uint8_t>, std::vector<uint8_t> > Gen(size_t alpha, size_t logn);
    bool Eval(const std::vector<uint8_t>& key, size_t x, size_t logn);
    BitVector EvalFull(const std::vector<uint8_t>& key, size_t logn);
    namespace prg {
        block getL(const block &seed);
        block getR(const block &seed);
    }
}
