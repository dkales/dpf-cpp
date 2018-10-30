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

int main(int argc, char** argv) {

    size_t N = 27;
    size_t iter = 100;
    benchEvalFull(N, iter);
    benchEvalFull8(N, iter);

    return 0;

}