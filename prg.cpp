#include "prg.h"

namespace dpf {
    using droidCrypto::mAesFixedKey;
    using droidCrypto::mAesFixedKey2;
    prg::prg() : aes_(droidCrypto::TestBlock) {

    }

    prg::prg(block seed) : aes_(seed) {

    }

    void prg::setSeed(block seed) {
        aes_.setKey(seed);
    }

    block prg::getL() {
        return aes_.encryptECB(droidCrypto::ZeroBlock);
    }

    block prg::getR() {
        return aes_.encryptECB(droidCrypto::LSBBlock);
    }

    block prg::getL(const block& seed) {
        return mAesFixedKey.encryptECB(seed);
    }

    block prg::getR(const block& seed) {
        return mAesFixedKey2.encryptECB(seed);
    }
//    std::array<block, 2> prg::getLR(const block& seed) {
//        std::array<block,2> blocks = {seed, seed^droidCrypto::MSBBlock};
//        mAesFixedKey.encryptECBBlocks(blocks.data(), 2, blocks.data());
//        return blocks;
//    }

    std::array<block, 2> prg::get() {
        std::array<block,2> blocks = {droidCrypto::ZeroBlock, droidCrypto::LSBBlock};
        aes_.encryptECBBlocks(blocks.data(), 2, blocks.data());
        return blocks;
    }
}