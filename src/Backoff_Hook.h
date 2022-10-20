#pragma once

#include "DKUtil/Hook.hpp"

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
		struct TreeCtors_extradata
		{
			void* func1;
			void* func2;
		};

		class CombatBehaviorTreeNode;
		using NodeArray = RE::BSTArray<CombatBehaviorTreeNode*>;

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(46731) };  //sub_1407D97D0
			trampoline.write_call<5>(Base.address() + 0xD7, WrapToRandomNode);
			INFO("Hook BackoffChance!");
		}

	private:
		static NodeArray& WrapToRandomNode(NodeArray& a_array, const char* a_name, TreeCtors_extradata* a_extradata, CombatBehaviorTreeNode* a_node);
	};
}