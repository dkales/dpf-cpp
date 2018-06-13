#pragma once
#include <droidCrypto/AES.h>

namespace dpf {
    using droidCrypto::AES;
    typedef droidCrypto::block block;

    class prg {
    public:
        prg();
        prg(block seed);

        void setSeed(block seed);

        block getL();
        block getR();
        std::array<block, 2> get();

        static block getL(const block& seed);
        static block getR(const block& seed);
        static std::array<block, 2> getLR(const block& seed);
    private:
        AES aes_;
    };
}
