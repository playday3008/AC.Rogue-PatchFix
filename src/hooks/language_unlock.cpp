#include "hooks/language_unlock.hpp"

#include <cstdint>

#include <atomic>

#include <Windows.h>

#include <injector/injector.hpp>

#include "config/language.hpp"
#include "logger.hpp" // IWYU pragma: keep

#include "hooks/registry/registry.hpp"

namespace hooks {
    namespace {
        std::atomic<bool> s_stop_watchdog {false};
        HANDLE            s_watchdog_thread = nullptr;

        uint32_t *s_menu_bf_global  = nullptr;
        uint32_t *s_audio_bf_global = nullptr;
        uint32_t *s_sub_bf_global   = nullptr;

        // Watchdog: wait for globals to be populated, then overwrite with all-languages.
        // No code modification — avoids triggering VMProtect integrity checks.
        // GetGameId is called once during startup and cached by the protection layer,
        // so overwriting after init is safe.
        auto CALLBACK watchdog_proc(LPVOID /*unused*/) -> DWORD {
            constexpr int      poll_ms     = 100;
            constexpr int      max_wait_ms = 30000;
            constexpr int      settle_ms   = 3000;
            constexpr uint32_t unlock_mask = lang::k_all_languages;

            for (int waited = 0; waited < max_wait_ms && !s_stop_watchdog.load();
                 waited += poll_ms) {
                Sleep(poll_ms);
                if (*s_menu_bf_global != 0)
                    break;
            }

            if (*s_menu_bf_global == 0) {
                log::get()->error("LanguageUnlockHook: bitfield globals never populated");
                return 1;
            }

            log::get()->trace("LanguageUnlockHook: original menu=0x{:X} sub=0x{:X} audio=0x{:X}",
                              *s_menu_bf_global,
                              *s_sub_bf_global,
                              *s_audio_bf_global);

            // Let GetGameId run with original values before we overwrite.
            Sleep(settle_ms);

            *s_menu_bf_global  = unlock_mask;
            *s_sub_bf_global   = unlock_mask;
            *s_audio_bf_global = unlock_mask;

            log::get()->info("LanguageUnlockHook: bitfields patched to 0x{:X}", unlock_mask);

            // Keep patching — the game may re-read localization.lang or DLC may clear bits.
            while (!s_stop_watchdog.load()) {
                Sleep(1000);
                if (*s_menu_bf_global != unlock_mask)
                    *s_menu_bf_global = unlock_mask;
                if (*s_sub_bf_global != unlock_mask)
                    *s_sub_bf_global = unlock_mask;
                if (*s_audio_bf_global != unlock_mask)
                    *s_audio_bf_global = unlock_mask;
            }

            return 0;
        }
    } // namespace

    auto HookTraits<LanguageUnlockHook>::install(const patterns::ResolvedAddresses &addrs) -> bool {
        log::get()->trace("LanguageUnlockHook: installing");

        const auto &cfg        = config<LanguageUnlockHook>();
        bool        unlock_all = cfg.unlock_all.get();

        if (!unlock_all) {
            log::get()->info("LanguageUnlockHook: disabled, skipping");
            return true;
        }

        auto lang_bf_write = addrs.lang_bf_write.value();

        // Resolve bitfield global addresses (read-only — no code modification).
        //   +0:  mov [rip+off], edi   → BF1 (menu)      — disp at +2
        //   +6:  mov [rip+off], ebx   → BF3 (audio)     — disp at +8
        //   +22: mov [rip+off], esi   → BF2 (subtitle)  — disp at +24
        s_menu_bf_global  = injector::ReadRelativeOffset(lang_bf_write + 2).get<uint32_t>();
        s_audio_bf_global = injector::ReadRelativeOffset(lang_bf_write + 8).get<uint32_t>();
        s_sub_bf_global   = injector::ReadRelativeOffset(lang_bf_write + 24).get<uint32_t>();

        log::get()->trace("LanguageUnlockHook: menu_bf=0x{:X} sub_bf=0x{:X} audio_bf=0x{:X}",
                          reinterpret_cast<uintptr_t>(s_menu_bf_global),
                          reinterpret_cast<uintptr_t>(s_sub_bf_global),
                          reinterpret_cast<uintptr_t>(s_audio_bf_global));

        s_stop_watchdog.store(false);
        s_watchdog_thread = CreateThread(nullptr, 0, watchdog_proc, nullptr, 0, nullptr);
        if (s_watchdog_thread == nullptr) {
            log::get()->error("LanguageUnlockHook: failed to create watchdog thread");
            return false;
        }

        log::get()->info("LanguageUnlockHook: watchdog started (data-only, no code patches)");
        return true;
    }
} // namespace hooks
