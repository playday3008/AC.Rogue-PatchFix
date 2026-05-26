#pragma once

#include <cstdint>

#include <array>
#include <optional>
#include <string_view>

namespace patterns {
    struct ResolvedAddresses {
        std::optional<uintptr_t> get_game_id;
        std::optional<uintptr_t> lang_bf_write;
    };

    struct ScanEntry {
        std::string_view         name;
        std::string_view         bytes;
        ptrdiff_t                offset;
        std::optional<uintptr_t> ResolvedAddresses::*field;
    };

    // clang-format off
    inline constexpr auto scan_entries = std::to_array<ScanEntry>({
        {.name="GET_GAME_ID",   .bytes="48 83 EC ? E8 ? ? ? ? 8B 15 ? ? ? ? 8B 0D", .offset=0x00, .field=&ResolvedAddresses::get_game_id},
        {.name="LANG_BF_WRITE", .bytes="89 3D ? ? ? ? 89 1D ? ? ? ? 89 05",         .offset=0x00, .field=&ResolvedAddresses::lang_bf_write},
    });
    // clang-format on

    [[nodiscard]] auto wait_for_code_ready() -> bool;
    [[nodiscard]] auto scan_all(ResolvedAddresses &out) -> bool;
} // namespace patterns
