#pragma once

#include "DKUtil/Hook.hpp"
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class BackoffStartHook
	{
		static float RescaleBackoffMinDistanceMult(RE::Actor* a_me, RE::Actor* a_he);

		// 1-6-640-0 @ 0x816350
		static constexpr std::uintptr_t AE_FuncID = 47920;
		static constexpr std::ptrdiff_t AE_OffsetL = 0x1BE;
		static constexpr std::ptrdiff_t AE_OffsetH = 0x1C6;
		// 1-5-97-0 @ 0x7D8C90
		static constexpr std::uintptr_t SE_FuncID = 46724;
		static constexpr std::ptrdiff_t SE_OffsetL = 0x201;
		static constexpr std::ptrdiff_t SE_OffsetH = 0x209;

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(SE_FuncID).address();

			Patch RelocatePointer{
				AsPointer(funcAddr + REL::Relocate(0x1B1, 0x173)),  //1407D8E41
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				REL::RelocationID(SE_FuncID, AE_FuncID).address(),
				REL::Relocate(std::make_pair(SE_OffsetL, SE_OffsetH), std::make_pair(AE_OffsetL, AE_OffsetH)),
				FUNC_INFO(RescaleBackoffMinDistanceMult),
				&RelocatePointer);

			handle->Enable();

			INFO("Hook BackoffStart");
		}
	};

	class BackoffChanceHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(46731, 47928) };  // 7D97D0, 816E40
			_WrapToRandomNode = trampoline.write_call<5>(Base.address() + REL::Relocate(0xD7, 0x1F0), WrapToRandomNode);
			INFO("Hook BackoffChance!");
		}

	private:
		static RE::NodeArray& WrapToRandomNode(RE::NodeArray& a_array, const char* a_name, RE::TreeCtors_extradata* a_extradata, RE::CombatBehaviorTreeNode* a_node);

		static inline REL::Relocation<decltype(WrapToRandomNode)> _WrapToRandomNode;
	};
}