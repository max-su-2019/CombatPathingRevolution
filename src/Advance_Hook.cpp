#include "Advance_Hook.h"
#include "Util.h"

namespace CombatPathing
{
	static constexpr char ENABLE_RADIUS_GV[] = "CPR_EnableAdvanceRadius",
						  INNER_MIN_GV[] = "CPR_InnerRadiusMin", INNER_MID_GV[] = "CPR_InnerRadiusMid", INNER_MAX_GV[] = "CPR_InnerRadiusMax",
						  OUTER_MIN_GV[] = "CPR_OuterRadiusMin", OUTER_MID_GV[] = "CPR_OuterRadiusMid", OUTER_MAX_GV[] = "CPR_OuterRadiusMax";

	static inline const float RescaleRadius(float a_delta, float min, float mid, float max)
	{
		return a_delta <= 0.0 ? min + (mid - min) * (a_delta + 1.0) : mid + (max - mid) * a_delta;
	}

	void AdvanceRadiusHook::RecalculateAdvanceRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker)
	{
		if (!a_radius || !a_target || !a_attacker || !IsMeleeOnly(a_attacker))
			return;

		bool enableAdvanceRadius;
		if (a_attacker->GetGraphVariableBool(ENABLE_RADIUS_GV, enableAdvanceRadius) && enableAdvanceRadius) {
			float InnerMin, InnerMid, InnerMax, OuterMin, OuterMid, OuterMax;
			if (a_attacker->GetGraphVariableFloat(INNER_MIN_GV, InnerMin) && a_attacker->GetGraphVariableFloat(INNER_MID_GV, InnerMid) && a_attacker->GetGraphVariableFloat(INNER_MAX_GV, InnerMax) &&
				a_attacker->GetGraphVariableFloat(OUTER_MIN_GV, OuterMin) && a_attacker->GetGraphVariableFloat(OUTER_MID_GV, OuterMid) && a_attacker->GetGraphVariableFloat(OUTER_MAX_GV, OuterMax)) {
				auto& inner = a_radius[0];
				auto& outer = a_radius[2];

				if (a_fullRadius) {
					inner = RescaleRadius(a_delta, InnerMin, InnerMid, InnerMax);
					outer = RescaleRadius(a_delta, OuterMin, OuterMid, OuterMax);
				} else {
					inner = InnerMid;
					outer = OuterMid;
				}
			}
		}
	}

	void AdvanceInterruptHook::Update(char** context)
	{
		static constexpr char INTERRUPT_ACTION_GV[] = "CPR_InterruptAction";
		auto me = CombatAI__get_me();
		if (me) {
			bool enableAdvanceRadius = false, interruptAction = false;
			if (me->GetGraphVariableBool(INTERRUPT_ACTION_GV, interruptAction) && interruptAction && me->GetGraphVariableBool(ENABLE_RADIUS_GV, enableAdvanceRadius) && enableAdvanceRadius) {
				char* path = *context;
				*(path + 0x14) = 5;
			}

			if (interruptAction)
				me->SetGraphVariableBool(INTERRUPT_ACTION_GV, false);
		}

		_Update(context);
	}

}