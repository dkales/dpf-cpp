#include "dpf.h"
#include "hashdatastore.h"

#include <chrono>
#include <iostream>

int main(int argc, char** argv) {

    size_t N = 27;
    std::chrono::duration<double> buildT, evalT, answerT;
    size_t keysizeT = 0;
    buildT = evalT = answerT = std::chrono::duration<double>::zero();

        auto time1 = std::chrono::high_resolution_clock::now();
        auto keys = DPF::Gen(234556, N);
        auto a = keys.first;
        keysizeT += a.size();
        auto time2 = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 1; i++) {
            std::vector<uint8_t> aaaa = DPF::EvalFull(a, N);
        }
        auto time3 = std::chrono::high_resolution_clock::now();

        buildT += time2 - time1;
        evalT += time3 - time2;
    //}
    std::cout << buildT.count() << "sec" << std::endl;
    std::cout << evalT.count() << "sec" << std::endl;

    return 0;

}