#include "Backoff_Hook.h"

namespace CombatPathing
{
	static constexpr char ENABLE_BACKOFF_GV[] = "CPR_EnableBackoff",
						  BACKOFF_MULT_GV[] = "CPR_BackoffMinDistMult", BACKOFF_CHANCE_GV[] = "CPR_BackoffChance";

	float BackoffStartHook::RescaleBackoffMinDistanceMult(RE::Actor* a_actor, RE::Actor*)
	{
		bool enablebackoff;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
			float backoffMult;
			if (a_actor->GetGraphVariableFloat(BACKOFF_MULT_GV, backoffMult))
				return backoffMult;
		}

		auto BackoffMinDistMultSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatBackoffMinDistanceMult");
		return BackoffMinDistMultSettings ? BackoffMinDistMultSettings->GetFloat() : 0.75f;
	}

	RE::NodeArray& BackoffChanceHook::WrapToRandomNode(RE::NodeArray& a_array, const char* a_name, RE::TreeCtors_extradata* a_extradata, RE::CombatBehaviorTreeNode* a_node)
	{
		static auto RecalculateBackoffChance = +[](RE::Actor* a_actor) -> float {
			bool enablebackoff;
			if (a_actor && a_actor->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
				float backoffChance;
				if (a_actor->GetGraphVariableFloat(BACKOFF_CHANCE_GV, backoffChance))
					return backoffChance;
			}

			auto BackoffChanceSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatBackoffChance");
			return BackoffChanceSettings ? BackoffChanceSettings->GetFloat() : 0.25f;
		};

		if (a_extradata) {
			a_extradata->func1 = RecalculateBackoffChance;
			a_extradata->func2 = nullptr;

			static REL::Relocation<decltype(WrapToRandomNode)> WrapToRandomNode_New{ REL::ID(46641) };
			return a_array = WrapToRandomNode_New(a_array, a_name, a_extradata, a_node); // ae change : array value is changed in-place instead of returning
		} else {
			ERROR("GetBackoffChance Redirect fail!");
			return a_array = _WrapToRandomNode(a_array, a_name, a_extradata, a_node);  // ae change : array value is changed in-place instead of returning
		}
	}

}
