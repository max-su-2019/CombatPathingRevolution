#pragma once
#include "DKUtil/Hook.hpp"
#include "RE/CombatBehaviorTreeControl.h"
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class CirclingChanceHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(49720, 50647) };

			_GetCirclingChance = trampoline.write_branch<5>(Base.address() + REL::Relocate(0x22, 0x2c), GetCirclingChance);
			INFO("Hook Circling!");
		}

	private:
		static float GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance);

		static inline REL::Relocation<decltype(GetCirclingChance)> _GetCirclingChance;
	};

	class AdvanceToCircleHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(46731) };  //sub_1407D97D0
			_PushBackNode = trampoline.write_call<5>(Base.address() + 0x493, PushBackNode);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static RE::NodeArray& PushBackNode(RE::NodeArray& a_master, RE::NodeArray& a_target);

		static inline REL::Relocation<decltype(PushBackNode)> _PushBackNode;

		static bool ShouldCircle(void* a_context);
	};
}