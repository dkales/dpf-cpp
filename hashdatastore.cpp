#include "hashdatastore.h"

#include <cassert>

hashdatastore::hash_type hashdatastore::answer_pir1(const std::vector<uint8_t> &indexing) const {
//    hash_type result = _mm256_set_epi64x(0,0,0,0);
//    for(size_t i = 0; i < data_.size(); i++) {
//        if(indexing.isSet(i))
//            result = _mm256_xor_si256(result, data_[i]);
//    }
//    return result;

    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8][2] = {
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint8_t tmp = indexing[i/8];
        results[0][(tmp>>0)&1] = _mm256_xor_si256(results[0][1], data_[i]);
        results[1][(tmp>>1)&1] = _mm256_xor_si256(results[1][1], data_[i+1]);
        results[2][(tmp>>2)&1] = _mm256_xor_si256(results[2][1], data_[i+2]);
        results[3][(tmp>>3)&1] = _mm256_xor_si256(results[3][1], data_[i+3]);
        results[4][(tmp>>4)&1] = _mm256_xor_si256(results[4][1], data_[i+4]);
        results[5][(tmp>>5)&1] = _mm256_xor_si256(results[5][1], data_[i+5]);
        results[6][(tmp>>6)&1] = _mm256_xor_si256(results[6][1], data_[i+6]);
        results[7][(tmp>>7)&1] = _mm256_xor_si256(results[7][1], data_[i+7]);
    }

    result = _mm256_xor_si256(results[0][1], results[1][1]);
    result = _mm256_xor_si256(result, results[2][1]);
    result = _mm256_xor_si256(result, results[3][1]);
    result = _mm256_xor_si256(result, results[4][1]);
    result = _mm256_xor_si256(result, results[5][1]);
    result = _mm256_xor_si256(result, results[6][1]);
    result = _mm256_xor_si256(result, results[7][1]);
    return result;
}

hashdatastore::hash_type hashdatastore::answer_pir2(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint64_t tmp = indexing[i/8];
        results[0] = _mm256_xor_si256(results[0], _mm256_and_si256(data_[i+0], _mm256_set1_epi64x(-((tmp>>0)&1))));
        results[1] = _mm256_xor_si256(results[1], _mm256_and_si256(data_[i+1], _mm256_set1_epi64x(-((tmp>>1)&1))));
        results[2] = _mm256_xor_si256(results[2], _mm256_and_si256(data_[i+2], _mm256_set1_epi64x(-((tmp>>2)&1))));
        results[3] = _mm256_xor_si256(results[3], _mm256_and_si256(data_[i+3], _mm256_set1_epi64x(-((tmp>>3)&1))));
        results[4] = _mm256_xor_si256(results[4], _mm256_and_si256(data_[i+4], _mm256_set1_epi64x(-((tmp>>4)&1))));
        results[5] = _mm256_xor_si256(results[5], _mm256_and_si256(data_[i+5], _mm256_set1_epi64x(-((tmp>>5)&1))));
        results[6] = _mm256_xor_si256(results[6], _mm256_and_si256(data_[i+6], _mm256_set1_epi64x(-((tmp>>6)&1))));
        results[7] = _mm256_xor_si256(results[7], _mm256_and_si256(data_[i+7], _mm256_set1_epi64x(-((tmp>>7)&1))));
    }

    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}
hashdatastore::hash_type hashdatastore::answer_pir3(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint64_t tmp = ((uint64_t) indexing[i/8]) << 56;
        __m256i x = _mm256_set1_epi64x(tmp);
        results[0] = _mm256_xor_si256(results[0], _mm256_maskload_epi64((const long long int *)&data_[i+7], x));
        x = _mm256_slli_si256(x, 1);
        results[1] = _mm256_xor_si256(results[1], _mm256_maskload_epi64((const long long int *)&data_[i+6], x));
        x = _mm256_slli_si256(x, 1);
        results[2] = _mm256_xor_si256(results[2], _mm256_maskload_epi64((const long long int *)&data_[i+5], x));
        x = _mm256_slli_si256(x, 1);
        results[3] = _mm256_xor_si256(results[3], _mm256_maskload_epi64((const long long int *)&data_[i+4], x));
        x = _mm256_slli_si256(x, 1);
        results[4] = _mm256_xor_si256(results[4], _mm256_maskload_epi64((const long long int *)&data_[i+3], x));
        x = _mm256_slli_si256(x, 1);
        results[5] = _mm256_xor_si256(results[5], _mm256_maskload_epi64((const long long int *)&data_[i+2], x));
        x = _mm256_slli_si256(x, 1);
        results[6] = _mm256_xor_si256(results[6], _mm256_maskload_epi64((const long long int *)&data_[i+1], x));
        x = _mm256_slli_si256(x, 1);
        results[7] = _mm256_xor_si256(results[7], _mm256_maskload_epi64((const long long int *)&data_[i+0], x));
    }

    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}
