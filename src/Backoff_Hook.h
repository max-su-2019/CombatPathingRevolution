#pragma once

#include "DKUtil/Hook.hpp"
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class BackoffStartHook
	{
		static float RescaleBackoffMinDistanceMult(RE::Actor* a_me, RE::Actor* a_he);

		static constexpr std::uintptr_t FuncID = 46724;   //1407D8C90
		static constexpr std::ptrdiff_t OffsetL = 0x201;  //1407D8E91
		static constexpr std::ptrdiff_t OffsetH = 0x209;  //1407D8E99

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();

			Patch RelocatePointer{
				AsPointer(funcAddr + 0x1b1),  //1407D8E41
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
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

			REL::Relocation<std::uintptr_t> Base{ REL::ID(46731) };  //sub_1407D97D0
			_WrapToRandomNode = trampoline.write_call<5>(Base.address() + 0xD7, WrapToRandomNode);
			INFO("Hook BackoffChance!");
		}

	private:
		static RE::NodeArray& WrapToRandomNode(RE::NodeArray& a_array, const char* a_name, RE::TreeCtors_extradata* a_extradata, RE::CombatBehaviorTreeNode* a_node);

		static inline REL::Relocation<decltype(WrapToRandomNode)> _WrapToRandomNode;
	};
}