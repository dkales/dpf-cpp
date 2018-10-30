#include "dpf.h"
#include "hashdatastore.h"

#include <chrono>
#include <iostream>


int testEvalFull8()  {

    size_t N = 11;
    auto keys = DPF::Gen(0, N);
    auto a = keys.first;
    std::vector<uint8_t> aaaa = DPF::EvalFull(a, N);
    std::vector<uint8_t> bbbb = DPF::EvalFull8(a, N);

    if(aaaa == bbbb) {
        return 0;
    }
    else {
        std::cout << "EvalFull and EvalFull8 differ\n";
        std::cout << "EvalFull size: " << aaaa.size() << " EvalFull8 size: " << bbbb.size() << std::endl;
        for(size_t i = 0; i < aaaa.size(); i++) {
            if(aaaa[i] != bbbb[i]) {
                std::cout << "at pos " << i << std::endl;
            }
        }
        return -1;
    }
}

int testCorr() {
    size_t N = 20;
    hashdatastore store;
    store.reserve(1ULL << N);
    for (size_t i = 0; i < (1ULL << N); i++) {
        store.push_back(_mm256_set_epi64x(i, i, i, i));
    }

    auto keys = DPF::Gen(123456, N);
    auto a = keys.first;
    auto b = keys.second;
    std::vector<uint8_t> aaaa = DPF::EvalFull8(a, N);
    std::vector<uint8_t> bbbb = DPF::EvalFull8(b, N);
    hashdatastore::hash_type answerA = store.answer_pir2(aaaa);
    hashdatastore::hash_type answerB = store.answer_pir2(bbbb);
    hashdatastore::hash_type answer = _mm256_xor_si256(answerA, answerB);
    if(_mm256_extract_epi64(answer, 0) == 123456) {
        return 0;
    } else {
        std::cout << "PIR answer wrong\n";
        return -1;
    }

}

int main(int argc, char** argv) {
    int res = 0;
    res |= testEvalFull8();
    res |= testCorr();
    return res;
}