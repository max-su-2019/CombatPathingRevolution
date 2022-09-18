#include "Circling_Hook.h"

namespace CombatPathing
{
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

}