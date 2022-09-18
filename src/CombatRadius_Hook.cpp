#include "CombatRadius_Hook.h"
#include "Functions.h"

namespace CombatPathing
{
	static inline const float RescaleRadius(float a_delta, float min, float mid, float max)
	{
		return a_delta <= 0.0 ? min + (mid - min) * (a_delta + 1.0) : mid + (max - mid) * a_delta;
	}

	void CombatRadiusHook::RecalculateCombatRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker)
	{
		static_assert(sizeof(float) == 0x4);

		if (!a_radius || !a_target || !a_attacker || !IsMeleeOnly(a_attacker)) {
			return;
		}

		auto& inner = a_radius[0];
		auto& outer = a_radius[2];

		if (a_fullRadius) {
			inner = RescaleRadius(a_delta, 50.f, 90.f, 250.f);
			outer = RescaleRadius(a_delta, 110.f, 200.f, 300.f);
		} else {
			inner = 90.f;
			outer = 200.f;
		}
	}

}