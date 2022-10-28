#include "Circling_Hook.h"

namespace CombatPathing
{
	static constexpr char ENABLE_CIRCLING_GV[] = "CPR_EnableCircling",
						  CIRCLING_MIN_GV[] = "CPR_CirclingDistMin", CIRCLING_MAX_GV[] = "CPR_CirclingDistMax";

	float CirclingHook::GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance, [[maybe_unused]] RE::Character* a_actor)
	{
		if (a_actor && a_actor->currentCombatTarget && a_actor->currentCombatTarget.get() && a_actor->combatController) {
			bool enableCircling;
			if (a_actor->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling) {
				float circlingDistMin, circlingDistMax;
				if (a_actor->GetGraphVariableFloat(CIRCLING_MIN_GV, circlingDistMin) && a_actor->GetGraphVariableFloat(CIRCLING_MAX_GV, circlingDistMax)) {
					static auto GetEquippementRange = [](RE::CombatInventory* a_inv) -> float {
						if (a_inv) {
							return a_inv->optimalRange;
						}

						return 0.f;
					};
					auto weaponRange = GetEquippementRange(a_actor->combatController->inventory);
					auto distance = a_actor->GetPosition().GetDistance(a_actor->currentCombatTarget.get()->GetPosition()) - a_actor->currentCombatTarget.get()->GetBoundRadius();
					if (distance < circlingDistMin + weaponRange || distance > 300.f + circlingDistMax)
						return std::max(0.1f, a_minChance);  //The chance must be a bit greater than zero, ohterwise NPC would be stucked by barriers.
				}
			}
		}

		return _GetCirclingChance(a_circleMult, a_minChance, a_maxChance, a_actor);
	}

}