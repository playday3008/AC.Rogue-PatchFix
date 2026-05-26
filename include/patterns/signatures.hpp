#pragma once

#include <cstdint>

#include <array>
#include <optional>
#include <string_view>

namespace patterns {
    struct ResolvedAddresses {
        std::optional<uintptr_t> viewport_ratio_load;
        std::optional<uintptr_t> viewport_ratio_mul;
        std::optional<uintptr_t> scaling_branch_start;
        std::optional<uintptr_t> scaling_branch_end;
        std::optional<uintptr_t> display_flag;
        std::optional<uintptr_t> fov_store;
        std::optional<uintptr_t> coord_transform;
        std::optional<uintptr_t> scaling_offsets;
        std::optional<uintptr_t> game_unpause;
        std::optional<uintptr_t> game_pause;
        std::optional<uintptr_t> game_pause2;
        std::optional<uintptr_t> get_game_id;
        std::optional<uintptr_t> lang_bf_write;
        std::optional<uintptr_t> lang_setup;
        std::optional<uintptr_t> get_language;
        std::optional<uintptr_t> fps_sleep_branch;
        std::optional<uintptr_t> fps_frame_time;
    };

    struct ScanEntry {
        std::string_view         name;
        std::string_view         bytes;
        ptrdiff_t                offset;
        std::optional<uintptr_t> ResolvedAddresses::*field;
    };

    // clang-format off
    inline constexpr auto scan_entries = std::to_array<ScanEntry>({
        {.name="VIEWPORT_RATIO_LOAD", .bytes="0F 28 E8 EB ? F3 0F 10 05 ? ? ? ? F3 0F 5E C1 F3 0F 59 C4",                                  .offset=0x05, .field=&ResolvedAddresses::viewport_ratio_load},
        {.name="VIEWPORT_RATIO_MUL",  .bytes="F3 0F 59 25 ? ? ? ? EB ? 0F 28 E5",                                                          .offset=0x00, .field=&ResolvedAddresses::viewport_ratio_mul},
        {.name="SCALING_BRANCH",      .bytes="45 0F 2F C1 41 0F 28 F0 41 0F 28 F9 76 ? 41 0F 28 F8 F3 0F 59 3D",                           .offset=0x00, .field=&ResolvedAddresses::scaling_branch_start},
        {.name="SCALING_BRANCH",      .bytes="45 0F 2F C1 41 0F 28 F0 41 0F 28 F9 76 ? 41 0F 28 F8 F3 0F 59 3D",                           .offset=0x52, .field=&ResolvedAddresses::scaling_branch_end},
        {.name="DISPLAY_FLAG",        .bytes="0F 2F C8 73 ? 8B D6 F3 0F 59 15",                                                            .offset=0x00, .field=&ResolvedAddresses::display_flag},
        {.name="FOV_STORE",           .bytes="89 43 40 EB 05 F3 0F 11 73 40 48 8D 54 24 40",                                               .offset=0x05, .field=&ResolvedAddresses::fov_store},
        {.name="COORD_TRANSFORM",     .bytes="0F 28 D0 F3 0F 59 15 ? ? ? ? 0F 2F CA 77",                                                   .offset=0x00, .field=&ResolvedAddresses::coord_transform},
        {.name="SCALING_OFFSETS",     .bytes="F3 0F 11 4C 24 30 F3 0F 59 05 ? ? ? ? F3 0F 11 44 24 34",                                    .offset=0x00, .field=&ResolvedAddresses::scaling_offsets},
        {.name="GAME_UNPAUSE",        .bytes="C6 81 C0 02 00 00 00 48 8B 91 90 02 00 00 48 8B D9",                                         .offset=0x00, .field=&ResolvedAddresses::game_unpause},
        {.name="GAME_PAUSE",          .bytes="48 C1 E1 20 48 C1 F9 3F 48 23 08 48 39 4A 18 75 08 41 C6 80 C0 02 00 00 01",                 .offset=0x11, .field=&ResolvedAddresses::game_pause},
        {.name="GAME_PAUSE2",         .bytes="48 39 46 18 75 ? C6 87 C0 02 00 00 01 48 8B 74 24",                                          .offset=0x06, .field=&ResolvedAddresses::game_pause2},
        {.name="GET_GAME_ID",         .bytes="48 83 EC 28 B9 ? ? ? ? E8 ? ? ? ? 84 C0 74 ? E8 ? ? ? ? 33 C9 84 C0 0F 95 C1 8D 81 ? ? ? ?", .offset=0x00, .field=&ResolvedAddresses::get_game_id},
        {.name="LANG_BF_WRITE",       .bytes="0F B6 44 24 ? 89 3D ? ? ? ? 89 1D ? ? ? ? 89 05",                                            .offset=0x05, .field=&ResolvedAddresses::lang_bf_write},
        {.name="LANG_SETUP",          .bytes="8B CB E8 ? ? ? ? E8 ? ? ? ? 8B C8 E8 ? ? ? ?",                                               .offset=0x00, .field=&ResolvedAddresses::lang_setup},
        {.name="GET_LANGUAGE",        .bytes="48 83 EC 28 8B 05 ? ? ? ? 83 F8 17 7C",                                                      .offset=0x00, .field=&ResolvedAddresses::get_language},
        {.name="FPS_SLEEP_BRANCH",    .bytes="48 89 43 78 48 2B BB 80 00 00 00 48 3B 43 70 73",                                            .offset=0x0F, .field=&ResolvedAddresses::fps_sleep_branch},
        {.name="FPS_FRAME_TIME",      .bytes="79 04 F3 0F 58 C1 F3 0F 59 05",                                                              .offset=0x06, .field=&ResolvedAddresses::fps_frame_time},
    });
    // clang-format on

    [[nodiscard]] auto scan_all(ResolvedAddresses &out) -> bool;
} // namespace patterns
