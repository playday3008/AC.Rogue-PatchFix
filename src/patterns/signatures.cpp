#include "patterns/signatures.hpp"

#include <cstddef>
#include <cstdint>

#include <expected>
#include <format>
#include <string>
#include <string_view>

#include <Windows.h>

#include <Hooking.Patterns.h>

#include "logger.hpp" // IWYU pragma: keep

[[nodiscard]] static auto
    find_unique(std::string_view name, std::string_view pat_str, ptrdiff_t offset)
        -> std::expected<uintptr_t, std::string> {
    auto pattern = hook::pattern(pat_str);
    if (pattern.size() != 1) {
        return std::unexpected(std::format("Pattern {}: {} (found {})",
                                           name,
                                           pattern.size() == 0 ? "NOT FOUND" : "AMBIGUOUS",
                                           pattern.size()));
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<uintptr_t>(pattern.get_first(offset));
}

auto patterns::wait_for_code_ready() -> bool {
    constexpr int max_attempts = 200;
    constexpr int poll_ms      = 50;

    auto probe = scan_entries[0].bytes;
    for (int i = 0; i < max_attempts; ++i) {
        Sleep(poll_ms);
        if (hook::pattern(probe).size() > 0) {
            log::get()->trace("Code section ready after {}ms", (i + 1) * poll_ms);
            return true;
        }
    }
    log::get()->error("Code section not ready after {}ms", max_attempts * poll_ms);
    return false;
}

auto patterns::scan_all(ResolvedAddresses &out) -> bool {
    out            = {};
    bool all_found = true;

    for (const auto &[name, bytes, offset, field] : scan_entries) {
        auto result = find_unique(name, bytes, offset);
        if (result) {
            out.*field = *result;
            log::get()->trace("Pattern {}: found at 0x{:X}", name, *result);
        } else {
            log::get()->error("{}", result.error());
            all_found = false;
        }
    }

    return all_found;
}
