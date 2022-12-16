#pragma once
#include "DKUtil/Hook.hpp"
#include "RE/CombatBehaviorTreeControl.h"
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	//Disable circling when the combat target not within circling distance range.
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

	//Insert a previous circle node before the advance node, to allow advance action switch to circle.
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

	class CircleAngleHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(49721) };  //sub_1408450A0
			_RescaleCircleAngle = trampoline.write_call<5>(Base.address() + 0x3A, RescaleCircleAngle);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float RescaleCircleAngle(const float a_circleMult, const float a_minAnlge, const float a_maxAngle);

		static inline REL::Relocation<decltype(RescaleCircleAngle)> _RescaleCircleAngle;
	};

	class CircleAngleHook2
	{
		static float GetMinCircleAngle();

		static constexpr std::uintptr_t FuncID = 49721;  //1408450A0
		static constexpr std::ptrdiff_t OffsetL = 0x3F;  //1408450DF
		static constexpr std::ptrdiff_t OffsetH = 0x47;  //1408450E7

		static constexpr Patch RelocateReturn{
			// movss xmm7, xmm0
			"\xF3\x0F\x10\xF8",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(GetMinCircleAngle),
				nullptr,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};

	class CircleAngleHook3
	{
		static float GetMaxCircleAngle();

		static constexpr std::uintptr_t FuncID = 46720;   //1407D8200
		static constexpr std::ptrdiff_t OffsetL = 0x2A2;  //1407D84A2
		static constexpr std::ptrdiff_t OffsetH = 0x2AA;  //1407D84AA

		static constexpr Patch RelocateReturn{
			// comiss xmm11, xmm0
			"\x44\x0F\x2F\xD8",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(GetMaxCircleAngle),
				nullptr,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};
}