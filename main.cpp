#include "dpf.h"
#include "hashdatastore.h"

#include <chrono>
#include <iostream>

int main(int argc, char** argv) {

    size_t N = 15;
    std::chrono::duration<double> buildT, evalT, answerT;
    size_t keysizeT = 0;
    buildT = evalT = answerT = std::chrono::duration<double>::zero();
    for(size_t N = 28; N > 5; N--) {

        hashdatastore store;
//    store.dummy(1ULL<<N);
        store.reserve(1ULL << N);
        for (size_t i = 0; i < (1ULL << N); i++) {
            store.push_back(_mm256_set_epi64x(i, i, i, i));
        }

        auto time1 = std::chrono::high_resolution_clock::now();
        auto keys = DPF::Gen(0, N);
        auto a = keys.first;
        auto b = keys.second;
//        std::cout << "keysize: " << a.size() << "bytes\n";
        keysizeT += a.size();
        auto time2 = std::chrono::high_resolution_clock::now();

//    bool aa = DPF::Eval(a, 1, N);
//    bool bb = DPF::Eval(b, 1, N);
//    std::cout << (aa^bb);

//    for(size_t i = 1; i < (1ULL<<N); i++) {
//        bool aaa = DPF::Eval(a, i, N);
//        bool bbb = DPF::Eval(b, i, N);
//        std::cout << (aaa ^ bbb);
//    }
//    std::cout << std::endl;

//    std::vector<uint8_t> aaaa = DPF::EvalFull(a, N);
    std::vector<uint8_t> aaaa;
    if(N > 10) {
        aaaa = DPF::EvalFull8(a, N);
    } else {
        aaaa = DPF::EvalFull(a, N);
    }

//    for(size_t ind = 0; ind < aaaa.size(); ind++) {
//        if (aaaa[ind] != aaaa8[ind]){
//            std::cout << "Wrong! " << ind << std::endl;
//            std::exit(-1);
//        }
//    }

//        std::vector<uint8_t> bbbb = DPF::EvalFull(b, N);
//    for(size_t i = 0; i < (1ULL<<N); i++) {
//        std::cout << int((aaaa^bbbb)[i]);
//    }
//    std::cout << std::endl;
        auto time3 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answerA = store.answer_pir2(aaaa);
//        hashdatastore::hash_type answerB = store.answer_pir2(bbbb);
//        hashdatastore::hash_type answer = _mm256_xor_si256(answerA, answerB);
        auto time4 = std::chrono::high_resolution_clock::now();
//        std::cout << _mm256_extract_epi64(answer, 0) << std::endl;

        buildT += time2 - time1;
        evalT += time3 - time2;
        answerT += time4 - time3;
    }
    std::cout << buildT.count() << "sec" << std::endl;
    std::cout << evalT.count() << "sec" << std::endl;
    std::cout << answerT.count() << "sec" << std::endl;
    std::cout << keysizeT + 32*(2<<5) + 32*22 << " bytes total transfer" << std::endl;

    return 0;

}
