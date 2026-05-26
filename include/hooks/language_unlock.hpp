#pragma once

#include <array>
#include <string_view>

#include "hooks/registry/config_base.hpp"
#include "hooks/registry/dep_list.hpp"
#include "hooks/registry/hook_traits.hpp"
#include "hooks/registry/ini_field.hpp"
#include "hooks/tags.hpp"

namespace hooks {
    template<>
    struct HookTraits<LanguageUnlockHook> {
        static constexpr std::string_view name = "LanguageUnlock";

        using hard_deps = dep_list<>;
        using soft_deps = dep_list<>;

        static constexpr auto required_patterns = std::array<PatternField, 1> {
            &patterns::ResolvedAddresses::lang_bf_write,
        };
        static constexpr auto optional_patterns = std::array<PatternField, 1> {
            &patterns::ResolvedAddresses::get_game_id,
        };

        struct Config : config_base<Config> {
            ini_field<bool>       unlock_all {"Language", "UnlockAll", false};
            static constexpr auto field_ptrs = std::tuple {&Config::unlock_all};
        };

        static auto install(const patterns::ResolvedAddresses &addrs) -> bool;
    };
} // namespace hooks
