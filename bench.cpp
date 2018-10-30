#include "dpf.h"
#include "hashdatastore.h"

#include <chrono>
#include <iostream>

void benchEvalFull(size_t N, size_t iter) {
    std::chrono::duration<double> buildT, evalT, answerT;
    buildT = evalT = answerT = std::chrono::duration<double>::zero();
    std::cout << "EvalFull, " << iter << " iterations" << std::endl;
    auto time1 = std::chrono::high_resolution_clock::now();
    auto keys = DPF::Gen(0, N);
    auto a = keys.first;
    auto time2 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < iter; i++) {
        std::vector<uint8_t> aaaa = DPF::EvalFull(a, N);
    }
    auto time3 = std::chrono::high_resolution_clock::now();

    buildT += time2 - time1;
    evalT += time3 - time2;
    std::cout << buildT.count() << "sec" << std::endl;
    std::cout << evalT.count() << "sec" << std::endl;
}

void benchEvalFull8(size_t N, size_t iter) {
    std::chrono::duration<double> buildT, evalT, answerT;
    buildT = evalT = answerT = std::chrono::duration<double>::zero();
    std::cout << "EvalFull8, " << iter << " iterations" << std::endl;
    auto time1 = std::chrono::high_resolution_clock::now();
    auto keys = DPF::Gen(0, N);
    auto a = keys.first;
    auto time2 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < iter; i++) {
        std::vector<uint8_t> aaaa = DPF::EvalFull8(a, N);
    }
    auto time3 = std::chrono::high_resolution_clock::now();

    buildT += time2 - time1;
    evalT += time3 - time2;
    std::cout << buildT.count() << "sec" << std::endl;
    std::cout << evalT.count() << "sec" << std::endl;
}

void benchAnswerPIR(size_t N, size_t iter) {
    std::array<std::chrono::duration<double>,6> answerT = {std::chrono::duration<double>::zero(), };
    std::cout << "AnswerPIR, " << iter << " iterations" << std::endl;
    auto keys = DPF::Gen(0, N);
    auto a = keys.first;
    hashdatastore store;
    store.reserve(1ULL << N);
    for (size_t i = 0; i < (1ULL << N); i++) {
        store.push_back(_mm256_set_epi64x(i, i, i, i));
    }
    std::vector<uint8_t> aaaa = DPF::EvalFull8(a, N);
    for(size_t i = 0; i < iter; i++) {
        auto time0 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answer0 = store.answer_pir_idea_speed_comparison(aaaa);
        auto time1 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answer1 = store.answer_pir1(aaaa);
        auto time2 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answer2 = store.answer_pir2(aaaa);
        auto time3 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answer3 = store.answer_pir3(aaaa);
        auto time4 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answer4 = store.answer_pir4(aaaa);
        auto time5 = std::chrono::high_resolution_clock::now();
        hashdatastore::hash_type answer5 = store.answer_pir4(aaaa);
        auto time6 = std::chrono::high_resolution_clock::now();
        answerT[0] += time1-time0;
        answerT[1] += time2-time1;
        answerT[2] += time3-time2;
        answerT[3] += time4-time3;
        answerT[4] += time5-time4;
        answerT[5] += time6-time5;
    }

    std::cout << "AnswerPIR ideal " << answerT[0].count() << "sec" << std::endl;
    std::cout << "AnswerPIR1      " << answerT[1].count() << "sec" << std::endl;
    std::cout << "AnswerPIR2      " << answerT[2].count() << "sec" << std::endl;
    std::cout << "AnswerPIR3      " << answerT[3].count() << "sec" << std::endl;
    std::cout << "AnswerPIR4      " << answerT[4].count() << "sec" << std::endl;
    std::cout << "AnswerPIR5      " << answerT[5].count() << "sec" << std::endl;
}


int main(int argc, char** argv) {

    size_t N = 27;
    size_t iter = 100;
//    benchEvalFull(N, iter);
//    benchEvalFull8(N, iter);
    benchAnswerPIR(25,100);

    return 0;

}