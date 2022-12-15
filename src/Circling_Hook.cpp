#include "Circling_Hook.h"
#include "RE/CombatBehaviorNodesMovement.h"
#include "Util.h"

namespace CombatPathing
{
	static constexpr char ENABLE_CIRCLING_GV[] = "CPR_EnableCircling",
						  CIRCLING_MIN_GV[] = "CPR_CirclingDistMin", CIRCLING_MAX_GV[] = "CPR_CirclingDistMax";

	float CirclingChanceHook::GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance)
	{
		auto me = CombatAI__get_me();
		auto he = CombatAI__get_he();
		if (me && he && me->combatController && me->combatController->combatStyle) {
			bool enableCircling;
			if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling) {
				float circlingDistMin, circlingDistMax;
				if (me->GetGraphVariableFloat(CIRCLING_MIN_GV, circlingDistMin) && me->GetGraphVariableFloat(CIRCLING_MAX_GV, circlingDistMax)) {
					auto weaponRange = GetEquippementRange(me->combatController->inventory);
					auto distance = me->GetPosition().GetDistance(he->GetPosition()) - he->GetBoundRadius();

					if (distance < circlingDistMin + weaponRange || distance > circlingDistMax)
						return std::max(0.1f, a_minChance);  //The chance must be a bit greater than zero, ohterwise NPC would be stucked by barriers.
				}
			}
		}

		return _GetCirclingChance(a_circleMult, a_minChance, a_maxChance);
	}

	RE::NodeArray& AdvanceToCircleHook::PushBackNode(RE::NodeArray& a_master, RE::NodeArray& a_target)
	{
		auto nodeCirlce = RE::NodeCloseMovementCircleDistant::createnew();
		if (nodeCirlce) {
			NodeArray array;
			RE::TreeCtors_extradata extraData;
			extraData.func1 = ShouldCircle;
			extraData.func2 = nullptr;

			a_master = pushback_parentof(a_master, wrap_to_conditional_2(array, "CPR Circle Distant", &extraData, nodeCirlce));
		}

		return _PushBackNode(a_master, a_target);
	}

	bool AdvanceToCircleHook::ShouldCircle(void* a_context)
	{
		auto me = CombatAI__get_me();
		auto he = CombatAI__get_he();
		if (me && he && me->combatController && me->combatController->combatStyle) {
			bool enableCircling;
			if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling) {
				float circlingDistMin, circlingDistMax;
				if (me->GetGraphVariableFloat(CIRCLING_MIN_GV, circlingDistMin) && me->GetGraphVariableFloat(CIRCLING_MAX_GV, circlingDistMax)) {
					auto weaponRange = GetEquippementRange(me->combatController->inventory);
					auto distance = me->GetPosition().GetDistance(he->GetPosition()) - he->GetBoundRadius();

					if (distance >= circlingDistMin + weaponRange && distance <= circlingDistMax + weaponRange) {
						auto circleMult = me->combatController->combatStyle->closeRangeData.circleMult;
						auto minCircleChance = GetGameSettingFloat("fCombatCircleChanceMin");
						auto maxCircleChance = GetGameSettingFloat("fCombatCircleChanceMax");

						if (minCircleChance.has_value() && maxCircleChance.has_value()) {
							auto chance = RescaleValue(circleMult, minCircleChance.value(), maxCircleChance.value());
							return Random::get(0.f, 1.0f) <= chance ? true : false;
						}
					}
				}
			}
		}

		return false;
	}

}