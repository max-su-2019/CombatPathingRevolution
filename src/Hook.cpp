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
			inner = RescaleRadius(a_delta, 60.f, 150.f, 250.f);
			outer = RescaleRadius(a_delta, 150.f, 250.f, 350.f);
		} else {
			inner = 60.f;
			outer = 150.f;
		}

		/*
		DEBUG("Set full radius: {}", a_fullRadius);
		DEBUG("Me: {} | {:X} <-> He: {} | {:X}", a_center->GetName(), a_center->GetFormID(), a_actor->GetName(), a_actor->GetFormID());
		DEBUG("InnerR: {} {:X} | OuterR {} {:X}", inner, AsAddress(&a_radius[0]), outer, AsAddress(&a_radius[2]));
		DEBUG("Delta: {}", a_delta);
		*/
	}

	float CirclingHook::GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance, [[maybe_unused]] RE::Character* a_actor)
	{
		if (a_actor && a_actor->currentCombatTarget && a_actor->currentCombatTarget.get()) {
			auto distance = a_actor->GetPosition().GetDistance(a_actor->currentCombatTarget.get()->GetPosition());
			if (distance < 160.f || distance > 550.f)
				return 0.f;
		}

		//DEBUG("Mult {} Min {} Max {}", a_circleMult, a_minChance, a_maxChance);
		//DEBUG("Actor: {} | {:X}", a_actor->GetName(), a_actor->GetFormID());

		return _GetCirclingChance(a_circleMult, a_minChance, a_maxChance, a_actor);
	}
}