#pragma once

#include <vector>
#include <x86intrin.h>

#include "alignment_allocator.h"


class BitVector;
class hashdatastore {
public:
    typedef __m256i hash_type;

    hashdatastore() = default;

    void reserve(size_t n) { data_.reserve(n); }
    void push_back(const hash_type& data) { data_.push_back(data); }
    void push_back(hash_type&& data) { data_.push_back(data); }

    void dummy(size_t n) { data_.resize(n, _mm256_set_epi64x(1,2,3,4)); }

    size_t size() const { return data_.size(); }

    hash_type answer_pir(const BitVector& indexing) const;


private:
    std::vector<hash_type, AlignmentAllocator<hash_type, sizeof(hash_type)> > data_;

};

