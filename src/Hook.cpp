#include "Hook.h"

namespace CombatPathing
{
	float CombatRadiusHook::RescaleRadius(float a_delta, float min, float mid, float max)
	{
		return a_delta <= 0.0 ? min + (mid - min) * (a_delta + 1.0) : mid + (max - mid) * a_delta;
	}

	void CombatRadiusHook::RecalculateCombatRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker)
	{
		static_assert(sizeof(float) == 0x4);

		if (!a_radius || !a_target || !a_attacker) {
			return;
		}

		auto& inner = a_radius[0];
		auto& outer = a_radius[2];

		if (a_fullRadius) {
			inner = RescaleRadius(a_delta, 50.f, 90.f, 250.f);
			outer = 200.f;
		} else {
			inner = 90.f;
			outer = 512.f;
		}
	}

	float CirclingHook::GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance, [[maybe_unused]] RE::Character* a_actor)
	{
		if (a_actor && a_actor->currentCombatTarget && a_actor->currentCombatTarget.get() && a_actor->combatController) {
			auto GetEquippementRange = [](RE::CombatInventory* a_inv) -> float {
				if (a_inv) {
					return a_inv->optimalRange;
				}

				return 0.f;
			};

			auto weaponRange = GetEquippementRange(a_actor->combatController->inventory);
			auto distance = a_actor->GetPosition().GetDistance(a_actor->currentCombatTarget.get()->GetPosition()) - a_actor->currentCombatTarget.get()->GetBoundRadius();
			if (distance < 40.f + weaponRange || distance > 300.f)
				return 0.f;
		}

		return _GetCirclingChance(a_circleMult, a_minChance, a_maxChance, a_actor);
	}

	float FallbackHook::GetFallbackDistance(RE::Actor* a_actor)
	{
		auto fallbackMult = a_actor->combatController->combatStyle->closeRangeData.fallbackMult;

		auto FallbackDistance = RescaleDistance(fallbackMult, 800.f, 1050.f);

		auto diameter = a_actor->GetBoundRadius() * 2.0f;

		return 1000.f;  //max(FallbackDistance, diameter);

		return _GetFallbackDistance(a_actor);
	}

	float FallbackHook::RescaleDistance(float a_mult, float a_min, float a_max)
	{
		return a_min + a_mult * (a_max - a_min);
	}
}