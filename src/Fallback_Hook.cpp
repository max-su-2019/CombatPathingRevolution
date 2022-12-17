#include "Fallback_Hook.h"
#include "Util.h"

namespace CombatPathing
{
	static constexpr char ENABLE_FALLBACK_GV[] = "CPR_EnableFallback",
						  FALLBACK_DIST_MIN_GV[] = "CPR_FallbackDistMin", FALLBACK_DIST_MAX_GV[] = "CPR_FallbackDistMax",
						  FALLBACK_TIME_MIN_GV[] = "CPR_FallbackWaitTimeMin", FALLBACK_TIME_MAX_GV[] = "CPR_FallbackWaitTimeMax";

	float FallbackDistanceHook1::GetFallbackDistance(RE::Actor* a_actor)
	{
		bool enableFallback;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackDistMin, fallbackDistMax;

			if (a_actor->GetGraphVariableFloat(FALLBACK_DIST_MIN_GV, fallbackDistMin) && a_actor->GetGraphVariableFloat(FALLBACK_DIST_MAX_GV, fallbackDistMax)) {
				auto fallbackMult = a_actor->GetActorRuntimeData().combatController->combatStyle->closeRangeData.fallbackMult;
				auto FallbackDistance = RescaleValue(fallbackMult, fallbackDistMin, fallbackDistMax);
				auto diameter = a_actor->GetBoundRadius() * 2.0f;

				return std::max(FallbackDistance, diameter);
			}
		}

		return _GetFallbackDistance(a_actor);
	}

	float FallbackDistanceHook2::GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor*)
	{
		bool enableFallback;
		if (a_me && a_me->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackDistMax;
			if (a_me->GetGraphVariableFloat(FALLBACK_DIST_MAX_GV, fallbackDistMax))
				return fallbackDistMax;
		}

		auto maxFallbackDistSettings = GetGameSettingFloat("fCombatFallbackDistanceMax");
		if (maxFallbackDistSettings.has_value())
			return maxFallbackDistSettings.value();

		return 256.f;
	}

	float FallbackWaitTimeHook1::GetFallbackWaitTime(RE::Actor* a_actor)
	{
		bool enableFallback;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackWaitTimeMin, fallbackWaitTimeMax;

			if (a_actor->GetGraphVariableFloat(FALLBACK_TIME_MIN_GV, fallbackWaitTimeMin) && a_actor->GetGraphVariableFloat(FALLBACK_TIME_MAX_GV, fallbackWaitTimeMax)) {
				auto fallbackMult = a_actor->GetActorRuntimeData().combatController->combatStyle->closeRangeData.fallbackMult;
				return RescaleValue(fallbackMult, fallbackWaitTimeMin, fallbackWaitTimeMax);
			}
		}

		return _GetFallbackWaitTime(a_actor);
	}

	float FallbackWaitTimeHook2::GetMinFallbackWaitTime(RE::Actor* a_me, RE::Actor*)
	{
		bool enableFallback;
		if (a_me && a_me->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackWaitTimeMin;
			if (a_me->GetGraphVariableFloat(FALLBACK_TIME_MIN_GV, fallbackWaitTimeMin))
				return fallbackWaitTimeMin;
		}

		auto minFallbackTimeSettings = GetGameSettingFloat("fCombatFallbackWaitTimeMin");
		if (minFallbackTimeSettings.has_value())
			return minFallbackTimeSettings.value();

		return 0.75f;
	}

}