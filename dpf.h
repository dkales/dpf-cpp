#pragma once

#include <cstdlib>
#include <vector>

#include "Defines.h"
namespace DPF {
    std::pair<std::vector<uint8_t>, std::vector<uint8_t> > Gen(size_t alpha, size_t logn);
    bool Eval(const std::vector<uint8_t>& key, size_t x, size_t logn);
    std::vector<uint8_t> EvalFull(const std::vector<uint8_t>& key, size_t logn);
    std::vector<uint8_t> EvalFull8(const std::vector<uint8_t>& key, size_t logn);
}
