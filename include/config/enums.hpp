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
    Hungarian   = 7,
    Italian     = 8,
    Japanese    = 9,
    Czech       = 10,
    Korean      = 11,
    Russian     = 12,
    Dutch       = 13,
    Danish      = 14,
    Norwegian   = 15,
    Swedish     = 16,
    Portuguese  = 17,
    Brazil      = 18,
    Finnish     = 19,
    Arabic      = 20,
    Mexican     = 21,
    LocTest     = 22,
    _count      = 23,
};
