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

	BackoffChanceHook::NodeArray& BackoffChanceHook::WrapToRandomNode(NodeArray& a_array, const char* a_name, TreeCtors_extradata* a_extradata, CombatBehaviorTreeNode* a_node)
	{
		static float (*RecalculateBackoffChance)(RE::Actor*) = [](RE::Actor* a_actor) -> float {
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
			return WrapToRandomNode_New(a_array, a_name, a_extradata, a_node);
		} else {
			ERROR("GetBackoffChance Redirect fail!");
			return _WrapToRandomNode(a_array, a_name, a_extradata, a_node);
		}
	}

}
