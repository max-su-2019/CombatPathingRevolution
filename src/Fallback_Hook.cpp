#include "Fallback_Hook.h"

namespace CombatPathing
{
	static constexpr char ENABLE_FALLBACK_GV[] = "CPR_EnableFallback",
						  FALLBACK_MIN_GV[] = "CPR_FallbackDistMin", FALLBACK_MAX_GV[] = "CPR_FallbackDistMax";

	static inline const float RescaleDistance(float a_mult, float a_min, float a_max)
	{
		return a_min + a_mult * (a_max - a_min);
	}

	float FallbackStartHook::GetFallbackDistance(RE::Actor* a_actor)
	{
		bool enableFallback;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackDistMin, fallbackDistMax;

			if (a_actor->GetGraphVariableFloat(FALLBACK_MIN_GV, fallbackDistMin) && a_actor->GetGraphVariableFloat(FALLBACK_MAX_GV, fallbackDistMax)) {
				auto fallbackMult = a_actor->combatController->combatStyle->closeRangeData.fallbackMult;
				auto FallbackDistance = RescaleDistance(fallbackMult, fallbackDistMin, fallbackDistMax);
				auto diameter = a_actor->GetBoundRadius() * 2.0f;

				return std::max(FallbackDistance, diameter);
			}
		}

		return _GetFallbackDistance(a_actor);
	}

	float FallbackUpdateHook::GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor*)
	{
		bool enableFallback;
		if (a_me && a_me->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackDistMax;
			if (a_me->GetGraphVariableFloat(FALLBACK_MAX_GV, fallbackDistMax))
				return fallbackDistMax;
		}

		auto maxFallbackSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatFallbackDistanceMax");
		if (maxFallbackSettings)
			return maxFallbackSettings->GetFloat();

		return 256.f;
	}

}