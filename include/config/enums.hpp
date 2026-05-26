#pragma once

#include <cstdint>

enum class Language : std::uint8_t {
    None        = 0,
    English     = 1,
    French      = 2,
    Spanish     = 3,
    Polish      = 4,
    German      = 5,
    ChineseTrad = 6,
    ChineseSimp = 7,
    Hungarian   = 8,
    Italian     = 9,
    Japanese    = 10,
    Czech       = 11,
    Korean      = 12,
    Russian     = 13,
    Dutch       = 14,
    Danish      = 15,
    Norwegian   = 16,
    Swedish     = 17,
    Portuguese  = 18,
    Brazil      = 19,
    Finnish     = 20,
    Arabic      = 21,
    Mexican     = 22,
    LocTest     = 23,
    _count      = 24,
};
