#include "dpf.h"
#include "Defines.h"
#include "PRNG.h"
#include "AES.h"
#include "Log.h"
#include <iostream>
#include <cassert>

namespace DPF {
    namespace prg {
        inline block getL(const block& seed) {
            return mAesFixedKey.encryptECB(seed);
        }

        inline block getR(const block& seed) {
            return mAesFixedKey2.encryptECB(seed);
        }
    }
    inline block clr(block in) {
        return in & ~MSBBlock;
    }
    inline bool getT(block in) {
        return !is_zero(in & MSBBlock);
    }
    inline bool ConvertBit(block in) {
        return !is_zero(in & LSBBlock);
    }
    inline block ConvertBlock(block in) {
        return mAesFixedKey.encryptECB(in);
    }

    std::pair<std::vector<uint8_t>, std::vector<uint8_t>> Gen(size_t alpha, size_t logn) {
        assert(logn <= 63);
        assert(alpha < (1<<logn));
        std::vector<uint8_t> ka, kb, CW;
        PRNG p = PRNG::getTestPRNG();
//        p.SetSeed(p.get<block>(), 2);
        block s0, s1;
        uint8_t t0, t1;
        p.get((uint8_t *) &s0, sizeof(s0));
        p.get((uint8_t *) &s1, sizeof(s1));
        t0 = getT(s0);
        t1 = !t0;

//        Log::v("before", s0);
//        Log::v("before", s1);
        s0 = clr(s0);
        s1 = clr(s1);

        ka.insert(ka.end(), (uint8_t*)&s0, ((uint8_t*)&s0) + sizeof(s0));
        ka.push_back(t0);
        kb.insert(kb.end(), (uint8_t*)&s1, ((uint8_t*)&s1) + sizeof(s1));
        kb.push_back(t1);
//        std::cout << ka.hex() << std::endl;
//        std::cout << kb.hex() << std::endl;
        size_t stop = logn >=7 ? logn - 7 : 0; // pack 7 layers in final CW
        for(size_t i = 0; i < stop; i++) {
            Log::v("gen", "%d, %d", t0, t1);
            Log::v("gen", s0);
            Log::v("gen", s1);

            block s0L = prg::getL(s0);
            uint8_t t0L = getT(s0L);
            s0L = clr(s0L);
            block s0R = prg::getR(s0);
            uint8_t t0R = getT(s0R);
            s0R = clr(s0R);

            block s1L = prg::getL(s1);
            uint8_t t1L = getT(s1L);
            s1L = clr(s1L);
            block s1R = prg::getR(s1);
            uint8_t t1R = getT(s1R);
            s1R = clr(s1R);

            if(alpha & (1ULL << (logn-1-i))) {
                //KEEP = R, LOSE = L
                block scw = s0L ^ s1L;
                uint8_t tLCW = t0L ^ t1L;
                uint8_t tRCW = t0R ^ t1R ^ 1;
                CW.insert(CW.end(), (uint8_t*)&scw, ((uint8_t*)&scw) + sizeof(scw));
                CW.push_back(tLCW);
                CW.push_back(tRCW);

                s0 = s0R;
                if(t0) s0 =  s0 ^ scw;
                s1 = s1R;
                if(t1) s1 =  s1 ^ scw;
                // new t
                if(t0) t0 = t0R ^ tRCW;
                else   t0 = t0R;
                if(t1) t1 = t1R ^ tRCW;
                else   t1 = t1R;
            }
            else {
                //KEEP = L, LOSE = R
                block scw = s0R ^ s1R;
                uint8_t tLCW = t0L ^ t1L ^ 1;
                uint8_t tRCW = t0R ^ t1R;
                CW.insert(CW.end(), (uint8_t*)&scw, ((uint8_t*)&scw) + sizeof(scw));
                CW.push_back(tLCW);
                CW.push_back(tRCW);
                //new s
                s0 = s0L;
                if(t0) s0 =  s0 ^ scw;
                s1 = s1L;
                if(t1) s1 =  s1 ^ scw;
                // new t
                if(t0) t0 = t0L ^ tLCW;
                else   t0 = t0L;
                if(t1) t1 = t1L ^ tLCW;
                else   t1 = t1L;
            }

        }
        reg_arr_union tmp = {ZeroBlock};
        tmp.arr[(alpha&127)/8] = (uint8_t)(1U<<((alpha&127)%8));
        tmp.reg = tmp.reg ^ ConvertBlock(s0) ^ ConvertBlock(s1);
        CW.insert(CW.end(), (uint8_t*)&tmp.reg, ((uint8_t*)&tmp.reg) + sizeof(tmp.reg));
        ka.insert(ka.end(), CW.begin(), CW.end());
        kb.insert(kb.end(), CW.begin(), CW.end());

        return std::make_pair(ka, kb);
    }

    bool Eval(const std::vector<uint8_t>& key, size_t x, size_t logn) {
        assert(logn <= 63);
        assert(x < (1<<logn));
        block s;
        memcpy(&s, key.data(), 16);
        uint8_t t = key.data()[16];
        size_t stop = logn >=7 ? logn - 7 : 0; // pack 7 layers in final CW
        for(size_t i = 0; i < stop; i++) {
            Log::v("eval", s);
            Log::v("eval", "t: %d", t);
            block sL = prg::getL(s);
            uint8_t tL = getT(sL);
            sL = clr(sL);
            block sR = prg::getR(s);
            uint8_t tR = getT(sR);
            sR = clr(sR);
            if(t) {
                block sCW;
                memcpy(&sCW, key.data() + 17 + i*18, 16);
                uint8_t tLCW = key.data()[17+i*18+16];
                uint8_t tRCW = key.data()[17+i*18+17];
                Log::v("eval", "tcw %d %d", tLCW, tRCW);
                tL^=tLCW;
                tR^=tRCW;
                sL^=sCW;
                sR^=sCW;
            }
            if(x & (1ULL<<(logn-1-i))) {
                s = sR;
                t = tR;
            } else {
                s = sL;
                t = tL;
            }
        }
        Log::v("evalfin", s);
        if(t) {
            reg_arr_union tmp;
            reg_arr_union CW;
            memcpy(CW.arr, key.data()+key.size()-16, 16);
            tmp.reg = CW.reg ^ ConvertBlock(s);
            return (tmp.arr[(x&127)/8] & (1UL << ((x&127)%8))) != 0;
        }
        else {
            reg_arr_union tmp;
            tmp.reg = ConvertBlock(s);
            return (tmp.arr[(x&127)/8] & (1UL << ((x&127)%8))) != 0;
        }
    }

    void EvalFullRecursive(const std::vector<uint8_t>& key, block s, uint8_t t, size_t lvl, size_t stop, std::vector<uint8_t>& res) {
        if(lvl == stop) {
            if(t) {
                reg_arr_union tmp;
                reg_arr_union CW;
                memcpy(CW.arr, key.data()+key.size()-16, 16);
                tmp.reg = CW.reg ^ ConvertBlock(s);
                res.insert(res.end(), &tmp.arr[0], &tmp.arr[16]);
            }
            else {
                reg_arr_union tmp;
                tmp.reg = ConvertBlock(s);
                res.insert(res.end(), &tmp.arr[0], &tmp.arr[16]);
            }
            return;
        }
        block sL = prg::getL(s);
        uint8_t tL = getT(sL);
        sL = clr(sL);
        block sR = prg::getR(s);
        uint8_t tR = getT(sR);
        sR = clr(sR);
        if(t) {
            block sCW;
            memcpy(&sCW, key.data() + 17 + lvl*18, 16);
            uint8_t tLCW = key.data()[17+lvl*18+16];
            uint8_t tRCW = key.data()[17+lvl*18+17];
            Log::v("eval", "tcw %d %d", tLCW, tRCW);
            tL^=tLCW;
            tR^=tRCW;
            sL^=sCW;
            sR^=sCW;
        }
        EvalFullRecursive(key, sL, tL, lvl+1, stop, res);
        EvalFullRecursive(key, sR, tR, lvl+1, stop, res);
    }

    std::vector<uint8_t> EvalFull(const std::vector<uint8_t>& key, size_t logn) {
        assert(logn <= 63);
        std::vector<uint8_t> data;
        data.reserve(1ULL << (logn-3));
        block s;
        memcpy(&s, key.data(), 16);
        uint8_t t = key.data()[16];
        size_t stop = logn >=7 ? logn - 7 : 0; // pack 7 layers in final CW
        EvalFullRecursive(key, s, t, 0, stop, data);
        return data;
    }
}
