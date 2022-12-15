#include "Util.h"

namespace CombatPathing
{
	using namespace RE;

	bool IsMeleeOnly(RE::Actor* a_actor)
	{
		using TYPE = RE::CombatInventoryItem::TYPE;

		if (!a_actor)
			return false;

		auto combatCtrl = a_actor->combatController;
		auto CombatInv = combatCtrl ? combatCtrl->inventory : nullptr;
		if (CombatInv) {
			for (const auto item : CombatInv->equippedItems) {
				if (item.item) {
					switch (item.item->GetType()) {
					case TYPE::kMagic:
					case TYPE::kRanged:
					case TYPE::kScroll:
					case TYPE::kStaff:
						return false;

					default:
						break;
					}
				}
			}

			return true;
		}

		return false;
	}

	float GetEquippementRange(RE::CombatInventory* a_inv)
	{
		if (a_inv) {
			return a_inv->optimalRange;
		}

		return 0.f;
	}

	std::optional<float> GetGameSettingFloat(const std::string a_name)
	{
		std::optional<float> result;

		auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());
		if (setting) {
			result.emplace(setting->GetFloat());
		}

		return result;
	}

	const float RescaleValue(float a_mult, float a_min, float a_max)
	{
		return a_min + a_mult * (a_max - a_min);
	}

	NodeArray& wrap_to_valuenode_13(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
	{
		// 1407DA290
		return _generic_foo<46736, NodeArray&, NodeArray&, const char*, TreeCtors_extradata*, CombatBehaviorTreeNode*>(a, name, extradata, node);
	}

	NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
	{
		// SkyrimSE.exe+7da060
		return _generic_foo<46732, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
	}

	NodeArray& init_withNode_withname(NodeArray& array, const char* name, CombatBehaviorTreeNode* node)
	{
		return _generic_foo<46261, NodeArray&, NodeArray&, const char*, CombatBehaviorTreeNode*>(array, name, node);
	}

	NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node)
	{
		return _generic_foo<46255, NodeArray&, NodeArray&, NodeArray&>(array, cont_node);
	}

	Character* CombatAI__get_he()
	{
		return _generic_foo<46265, RE::Character*>();  // SkyrimSE.exe+7c7b20
	}

	Character* CombatAI__get_me()
	{
		return _generic_foo<46264, RE::Character*>();  // SkyrimSE.exe+7C7A40
	}

}