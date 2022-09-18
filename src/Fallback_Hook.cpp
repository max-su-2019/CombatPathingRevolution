#include "Fallback_Hook.h"

namespace CombatPathing
{

	static inline const float RescaleDistance(float a_mult, float a_min, float a_max)
	{
		return a_min + a_mult * (a_max - a_min);
	}

	float FallbackStartHook::GetFallbackDistance(RE::Actor* a_actor)
	{
		auto fallbackMult = a_actor->combatController->combatStyle->closeRangeData.fallbackMult;

		auto FallbackDistance = RescaleDistance(fallbackMult, 800.f, 1050.f);

		auto diameter = a_actor->GetBoundRadius() * 2.0f;

		return max(FallbackDistance, diameter);

		return _GetFallbackDistance(a_actor);
	}

	float FallbackUpdateHook::GetMaxFallbackDistance(RE::Character* a_me, RE::Character* a_he)
	{
		if (!a_me || !a_he) {
			return 0.f;
		}

		DEBUG("ME: {}|0x{:08X} <-> HE: {}|0x{:08X}", a_me->GetName(), a_me->GetFormID(), a_he->GetName(), a_he->GetFormID());

		return 1000.f;
	}

}