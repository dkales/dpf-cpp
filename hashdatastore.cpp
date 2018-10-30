#include "hashdatastore.h"

#include <cassert>

const hashdatastore::hash_type precomputed_masks[256][8] = {
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0x00), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
        {_mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF), _mm256_set1_epi8(0xFF),},
};

hashdatastore::hash_type hashdatastore::answer_pir1(const std::vector<uint8_t> &indexing) const {
//    hash_type result = _mm256_set_epi64x(0,0,0,0);
//    for(size_t i = 0; i < data_.size(); i++) {
//        if(indexing.isSet(i))
//            result = _mm256_xor_si256(result, data_[i]);
//    }
//    return result;

    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8][2] = {
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result},
            {result, result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint8_t tmp = indexing[i/8];
        results[0][(tmp>>0)&1] = _mm256_xor_si256(results[0][1], data_[i]);
        results[1][(tmp>>1)&1] = _mm256_xor_si256(results[1][1], data_[i+1]);
        results[2][(tmp>>2)&1] = _mm256_xor_si256(results[2][1], data_[i+2]);
        results[3][(tmp>>3)&1] = _mm256_xor_si256(results[3][1], data_[i+3]);
        results[4][(tmp>>4)&1] = _mm256_xor_si256(results[4][1], data_[i+4]);
        results[5][(tmp>>5)&1] = _mm256_xor_si256(results[5][1], data_[i+5]);
        results[6][(tmp>>6)&1] = _mm256_xor_si256(results[6][1], data_[i+6]);
        results[7][(tmp>>7)&1] = _mm256_xor_si256(results[7][1], data_[i+7]);
    }

    result = _mm256_xor_si256(results[0][1], results[1][1]);
    result = _mm256_xor_si256(result, results[2][1]);
    result = _mm256_xor_si256(result, results[3][1]);
    result = _mm256_xor_si256(result, results[4][1]);
    result = _mm256_xor_si256(result, results[5][1]);
    result = _mm256_xor_si256(result, results[6][1]);
    result = _mm256_xor_si256(result, results[7][1]);
    return result;
}

hashdatastore::hash_type hashdatastore::answer_pir2(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint64_t tmp = indexing[i/8];
        results[0] = _mm256_xor_si256(results[0], _mm256_and_si256(data_[i+0], _mm256_set1_epi64x(-((tmp>>0)&1))));
        results[1] = _mm256_xor_si256(results[1], _mm256_and_si256(data_[i+1], _mm256_set1_epi64x(-((tmp>>1)&1))));
        results[2] = _mm256_xor_si256(results[2], _mm256_and_si256(data_[i+2], _mm256_set1_epi64x(-((tmp>>2)&1))));
        results[3] = _mm256_xor_si256(results[3], _mm256_and_si256(data_[i+3], _mm256_set1_epi64x(-((tmp>>3)&1))));
        results[4] = _mm256_xor_si256(results[4], _mm256_and_si256(data_[i+4], _mm256_set1_epi64x(-((tmp>>4)&1))));
        results[5] = _mm256_xor_si256(results[5], _mm256_and_si256(data_[i+5], _mm256_set1_epi64x(-((tmp>>5)&1))));
        results[6] = _mm256_xor_si256(results[6], _mm256_and_si256(data_[i+6], _mm256_set1_epi64x(-((tmp>>6)&1))));
        results[7] = _mm256_xor_si256(results[7], _mm256_and_si256(data_[i+7], _mm256_set1_epi64x(-((tmp>>7)&1))));
    }

    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}
hashdatastore::hash_type hashdatastore::answer_pir3(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint64_t tmp = ((uint64_t) indexing[i/8]) << 56;
        __m256i x = _mm256_set1_epi64x(tmp);
        results[0] = _mm256_xor_si256(results[0], _mm256_maskload_epi64((const long long int *)&data_[i+7], x));
        x = _mm256_slli_si256(x, 1);
        results[1] = _mm256_xor_si256(results[1], _mm256_maskload_epi64((const long long int *)&data_[i+6], x));
        x = _mm256_slli_si256(x, 1);
        results[2] = _mm256_xor_si256(results[2], _mm256_maskload_epi64((const long long int *)&data_[i+5], x));
        x = _mm256_slli_si256(x, 1);
        results[3] = _mm256_xor_si256(results[3], _mm256_maskload_epi64((const long long int *)&data_[i+4], x));
        x = _mm256_slli_si256(x, 1);
        results[4] = _mm256_xor_si256(results[4], _mm256_maskload_epi64((const long long int *)&data_[i+3], x));
        x = _mm256_slli_si256(x, 1);
        results[5] = _mm256_xor_si256(results[5], _mm256_maskload_epi64((const long long int *)&data_[i+2], x));
        x = _mm256_slli_si256(x, 1);
        results[6] = _mm256_xor_si256(results[6], _mm256_maskload_epi64((const long long int *)&data_[i+1], x));
        x = _mm256_slli_si256(x, 1);
        results[7] = _mm256_xor_si256(results[7], _mm256_maskload_epi64((const long long int *)&data_[i+0], x));
    }

    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}

hashdatastore::hash_type hashdatastore::answer_pir4(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        switch(indexing[i / 8]){
            case 0:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 1:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 2:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 3:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 4:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 5:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 6:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 7:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 8:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 9:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 10:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 11:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 12:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 13:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 14:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 15:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 16:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 17:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 18:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 19:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 20:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 21:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 22:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 23:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 24:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 25:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 26:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 27:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 28:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 29:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 30:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 31:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 32:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 33:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 34:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 35:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 36:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 37:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 38:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 39:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 40:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 41:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 42:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 43:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 44:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 45:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 46:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 47:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 48:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 49:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 50:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 51:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 52:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 53:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 54:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 55:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 56:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 57:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 58:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 59:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 60:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 61:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 62:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 63:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 64:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 65:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 66:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 67:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 68:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 69:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 70:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 71:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 72:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 73:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 74:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 75:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 76:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 77:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 78:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 79:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 80:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 81:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 82:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 83:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 84:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 85:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 86:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 87:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 88:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 89:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 90:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 91:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 92:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 93:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 94:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 95:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 96:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 97:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 98:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 99:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 100:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 101:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 102:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 103:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 104:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 105:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 106:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 107:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 108:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 109:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 110:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 111:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 112:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 113:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 114:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 115:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 116:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 117:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 118:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 119:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 120:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 121:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 122:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 123:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 124:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 125:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 126:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 127:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                //results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 128:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 129:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 130:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 131:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 132:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 133:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 134:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 135:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 136:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 137:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 138:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 139:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 140:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 141:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 142:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 143:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 144:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 145:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 146:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 147:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 148:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 149:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 150:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 151:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 152:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 153:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 154:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 155:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 156:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 157:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 158:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 159:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 160:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 161:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 162:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 163:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 164:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 165:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 166:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 167:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 168:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 169:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 170:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 171:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 172:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 173:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 174:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 175:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 176:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 177:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 178:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 179:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 180:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 181:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 182:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 183:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 184:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 185:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 186:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 187:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 188:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 189:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 190:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 191:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                //results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 192:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 193:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 194:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 195:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 196:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 197:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 198:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 199:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 200:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 201:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 202:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 203:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 204:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 205:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 206:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 207:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 208:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 209:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 210:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 211:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 212:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 213:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 214:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 215:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 216:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 217:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 218:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 219:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 220:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 221:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 222:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 223:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                //results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 224:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 225:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 226:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 227:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 228:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 229:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 230:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 231:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 232:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 233:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 234:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 235:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 236:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 237:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 238:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 239:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                //results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 240:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 241:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 242:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 243:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 244:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 245:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 246:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 247:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                //results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 248:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 249:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 250:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 251:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                //results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 252:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 253:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                //results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 254:
                //results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;


            case 255:
                results[0] = _mm256_xor_si256(results[0], data_[i+0]);
                results[1] = _mm256_xor_si256(results[1], data_[i+1]);
                results[2] = _mm256_xor_si256(results[2], data_[i+2]);
                results[3] = _mm256_xor_si256(results[3], data_[i+3]);
                results[4] = _mm256_xor_si256(results[4], data_[i+4]);
                results[5] = _mm256_xor_si256(results[5], data_[i+5]);
                results[6] = _mm256_xor_si256(results[6], data_[i+6]);
                results[7] = _mm256_xor_si256(results[7], data_[i+7]);
                break;
        }

    }
    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}

hashdatastore::hash_type hashdatastore::answer_pir5(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        uint64_t ind = indexing[i/8];
        const hash_type* masks = &precomputed_masks[ind][0];
        results[0] = _mm256_xor_si256(results[0], _mm256_and_si256(data_[i+0], masks[0]));
        results[1] = _mm256_xor_si256(results[1], _mm256_and_si256(data_[i+1], masks[1]));
        results[2] = _mm256_xor_si256(results[2], _mm256_and_si256(data_[i+2], masks[2]));
        results[3] = _mm256_xor_si256(results[3], _mm256_and_si256(data_[i+3], masks[3]));
        results[4] = _mm256_xor_si256(results[4], _mm256_and_si256(data_[i+4], masks[4]));
        results[5] = _mm256_xor_si256(results[5], _mm256_and_si256(data_[i+5], masks[5]));
        results[6] = _mm256_xor_si256(results[6], _mm256_and_si256(data_[i+6], masks[6]));
        results[7] = _mm256_xor_si256(results[7], _mm256_and_si256(data_[i+7], masks[7]));
    }

    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}

hashdatastore::hash_type hashdatastore::answer_pir_idea_speed_comparison(const std::vector<uint8_t> &indexing) const {
    hash_type result = _mm256_set_epi64x(0,0,0,0);
    hash_type results[8] = {
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result},
            {result}, };
    assert(data_.size() % 8 == 0);
    for(size_t i = 0; i < data_.size(); i+=8) {
        results[0] = _mm256_xor_si256(results[0], data_[i+0]);
        results[1] = _mm256_xor_si256(results[1], data_[i+1]);
        results[2] = _mm256_xor_si256(results[2], data_[i+2]);
        results[3] = _mm256_xor_si256(results[3], data_[i+3]);
        results[4] = _mm256_xor_si256(results[4], data_[i+4]);
        results[5] = _mm256_xor_si256(results[5], data_[i+5]);
        results[6] = _mm256_xor_si256(results[6], data_[i+6]);
        results[7] = _mm256_xor_si256(results[7], data_[i+7]);
    }

    result = _mm256_xor_si256(results[0], results[1]);
    result = _mm256_xor_si256(result, results[2]);
    result = _mm256_xor_si256(result, results[3]);
    result = _mm256_xor_si256(result, results[4]);
    result = _mm256_xor_si256(result, results[5]);
    result = _mm256_xor_si256(result, results[6]);
    result = _mm256_xor_si256(result, results[7]);
    return result;
}
