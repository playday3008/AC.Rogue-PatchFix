#pragma once

#include "hooks/tags.hpp"
#include "hooks/registry/dep_list.hpp"

namespace hooks {
    using AllHooks = hook_list<LanguageUnlockHook,
                               GameStateHook,
                               ViewportFittingHook,
                               ViewportScalingHook,
                               DisplayDetectionHook,
                               FOVCorrectionHook,
                               FPSUnlockHook>;
} // namespace hooks
