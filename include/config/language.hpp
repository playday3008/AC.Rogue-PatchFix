#pragma once

#include <cstdint>

#include <array>
#include <string_view>
#include <utility>

#include "config/enums.hpp"

namespace lang {
    constexpr auto bit(Language l) -> uint32_t {
        return 1U << std::to_underlying(l);
    }

    constexpr auto has(uint32_t bitfield, Language l) -> bool {
        return (bitfield & bit(l)) != 0;
    }

    consteval auto make_all_languages() -> uint32_t {
        uint32_t mask = 0;
        for (auto i = std::to_underlying(Language::English);
             i < std::to_underlying(Language::_count);
             ++i) {
            mask |= (1U << i);
        }
        return mask;
    }

    inline constexpr uint32_t k_all_languages = make_all_languages();
    static_assert(k_all_languages == 0x00FFFFFEU);

    // clang-format off
    inline constexpr auto k_names = std::to_array<std::pair<std::string_view, Language>>({
        {"None",        Language::None},
        {"English",     Language::English},
        {"French",      Language::French},
        {"Spanish",     Language::Spanish},
        {"Polish",      Language::Polish},
        {"German",      Language::German},
        {"ChineseTrad", Language::ChineseTrad},
        {"ChineseSimp", Language::ChineseSimp},
        {"Hungarian",   Language::Hungarian},
        {"Italian",     Language::Italian},
        {"Japanese",    Language::Japanese},
        {"Czech",       Language::Czech},
        {"Korean",      Language::Korean},
        {"Russian",     Language::Russian},
        {"Dutch",       Language::Dutch},
        {"Danish",      Language::Danish},
        {"Norwegian",   Language::Norwegian},
        {"Swedish",     Language::Swedish},
        {"Portuguese",  Language::Portuguese},
        {"Brazil",      Language::Brazil},
        {"Finnish",     Language::Finnish},
        {"Arabic",      Language::Arabic},
        {"Mexican",     Language::Mexican},
        {"LocTest",     Language::LocTest},
    });
    // clang-format on
} // namespace lang
