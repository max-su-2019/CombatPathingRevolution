#pragma once
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace RE;

	bool IsMeleeOnly(RE::Actor* a_actor);

	float GetEquippementRange(RE::CombatInventory* a_inv);

	std::optional<float> GetGameSettingFloat(const std::string a_name);

	const float RescaleValue(float a_mult, float a_min, float a_max);

	NodeArray& wrap_to_valuenode_13(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node);

	NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node);

	NodeArray& init_withNode_withname(NodeArray& array, const char* name, CombatBehaviorTreeNode* node);

	NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node);

	Character* CombatAI__get_he();

	Character* CombatAI__get_me();
}