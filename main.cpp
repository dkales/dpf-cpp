#include "dpf.h"
#include "hashdatastore.h"

#include <chrono>
#include <iostream>

using dpf::DPF;

int main(int argc, char** argv) {

    size_t N = 27;

    hashdatastore store;
    store.dummy(1ULL<<N);

    auto time1 = std::chrono::high_resolution_clock::now();
    auto keys = DPF::Gen(1, N);
    auto a = keys.first;
    auto b = keys.second;
    std::cout << "keysize: " << a.size() << "bytes\n";
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

    BitVector aaaa = DPF::EvalFull(a, N);
//    BitVector bbbb = DPF::EvalFull(b, N);
//    for(size_t i = 0; i < (1ULL<<N); i++) {
//        std::cout << int((aaaa^bbbb)[i]);
//    }
//    std::cout << std::endl;
    auto time3 = std::chrono::high_resolution_clock::now();
    hashdatastore::hash_type answer = store.answer_pir(aaaa);
    auto time4 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed1 = time2-time1;
    std::cout << elapsed1.count() << "sec" << std::endl;
    std::chrono::duration<double> elapsed2 = time3-time2;
    std::cout << elapsed2.count() << "sec" << std::endl;
    std::chrono::duration<double> elapsed3 = time4-time3;
    std::cout << elapsed3.count() << "sec" << std::endl;

    return 0;

}