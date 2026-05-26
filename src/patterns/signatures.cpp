#include "patterns/signatures.hpp"

#include <cstddef>
#include <cstdint>

#include <expected>
#include <format>
#include <string>
#include <string_view>

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
