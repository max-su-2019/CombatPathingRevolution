#pragma once

namespace RE
{
	class CombatBehaviorTreeNode;

	class CombatBehaviorTreeControl
	{
	public:
		struct ActionStack
		{
			struct ActionStackEntry
			{
				CombatBehaviorTreeNode* node_0;
				int num_8;
				int mb_pad_C;
			};

			uint32_t capacity_n_flags;
			int mb_pad_4;
			ActionStackEntry data[16];
			int size;
			int mb_pad_10C;
		};

		struct AI_Tree_Control_anotherControl
		{
			CombatBehaviorTreeControl* control;
			unsigned int c;
			char __pad_C[4];
		};

		enum Control_AI_States
		{
			switched_to_parent,
			processing,
			switched_to_children
		};

		void Ascend()
		{
			return _generic_foo<46229, void, CombatBehaviorTreeControl*>(this);  // 1407C69D0
		}

		void SetFailed(bool a_fail)
		{
			return _generic_foo<46240, void, CombatBehaviorTreeControl*, bool>(this, a_fail);  // 1407C6D30
		}

		BSTArrayHeapAllocator unk00;                     //00
		ActionStack action_stack;                        //10
		unsigned int size_120;                           //120
		int field_124;                                   //124
		AI_Tree_Control_anotherControl another_control;  //128
		CombatBehaviorTreeNode* cur_node;                //138
		CombatBehaviorTreeNode* node_another;            //140
		bool shouldInterrupt;                            //148
		Control_AI_States current_state;                 //14C
		char field_150[8];                               //150
		CombatController* master_controller;             //158
	};
	static_assert(offsetof(class CombatBehaviorTreeControl, master_controller) == 0x158);

}