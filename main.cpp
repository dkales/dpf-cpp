#include "dpf.h"
#include "hashdatastore.h"

#include <chrono>
#include <iostream>

int main(int argc, char** argv) {

    if(argc != 2) {
	    std::cout << "Usage: ./dpf_pir <log_tree_size>" << std::endl;
        return -1;
    }
    size_t NN = std::strtoull(argv[1], nullptr, 10);
    std::chrono::duration<double> buildT, evalT, answerT;
    size_t keysizeT = 0;
    buildT = evalT = answerT = std::chrono::duration<double>::zero();
    for(size_t N = NN; N > 0; N--) {

        hashdatastore store;
        store.reserve(1ULL << N);
        // Fill Datastore with dummy elements for benchmark
        for (size_t i = 0; i < (1ULL << N); i++) {
            store.push_back(_mm256_set_epi64x(i, i, i, i));
        }
        auto time1 = std::chrono::high_resolution_clock::now();
        auto keys = DPF::Gen(0, N);
        auto a = keys.first;
        auto b = keys.second;
        keysizeT += a.size();
        auto time2 = std::chrono::high_resolution_clock::now();

        std::vector<uint8_t> aaaa;
        if(N > 10) {
            aaaa = DPF::EvalFull8(a, N);
        } else {
            aaaa = DPF::EvalFull(a, N);
        }
        //std::vector<uint8_t> bbbb = DPF::EvalFull(b, N);

        auto time3 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answerA = store.answer_pir2(aaaa);
        //hashdatastore::hash_type answerB = store.answer_pir2(bbbb);
        //hashdatastore::hash_type answer = _mm256_xor_si256(answerA, answerB);
        auto time4 = std::chrono::high_resolution_clock::now();
        //std::cout << _mm256_extract_epi64(answer, 0) << std::endl;

        buildT += time2 - time1;
        evalT += time3 - time2;
        answerT += time4 - time3;
    }
    std::cout << "DPF.Gen: "        <<  buildT.count() << "sec" << std::endl;
    std::cout << "DPF.Eval: "       << evalT.count() << "sec" << std::endl;
    std::cout << "Inner Prod: "     << answerT.count() << "sec" << std::endl;
    std::cout << keysizeT << "; "   <<  NN*32 << " bytes total transfer" << std::endl;

    return 0;

}
