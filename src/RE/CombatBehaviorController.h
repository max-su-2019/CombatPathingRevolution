#pragma once
#include "CombatBehaviorTreeControl.h"

namespace RE
{
	class CombatBehaviorController
	{
	public:
		CombatBehaviorTreeControl* control_00;             // 00
		BSTArray<CombatBehaviorTreeControl*> controls_08;  // 08
		void* unk20;                                       // 20
		ActorHandle actorHandle;                           // 28
		ActorHandle targetHandle;                          // 2C
		ActorHandle previousTargetHandle;                  // 30
		char unk34[36];                                    // 34
		BSTArray<CombatBehaviorTreeControl*> controls_58;  // 58
	};
	static_assert(offsetof(class CombatBehaviorController, controls_58) == 0x58);
}