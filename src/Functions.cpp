#include "Functions.h"
namespace CombatPathing
{
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

}