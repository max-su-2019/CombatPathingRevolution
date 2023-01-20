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

			_GetCirclingChance = trampoline.write_branch<5>(Base.address() + REL::Relocate(0x22, 0x2C), GetCirclingChance);
			INFO("{} Done!", __FUNCTION__);
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

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(49721, 50648) };  //sub_1408450A0
			_RescaleCircleAngle = trampoline.write_call<5>(Base.address() + REL::Relocate(0x3A, 0x44), RescaleCircleAngle);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float RescaleCircleAngle(const float a_circleMult, const float a_minAnlge, const float a_maxAngle);

		static inline REL::Relocation<decltype(RescaleCircleAngle)> _RescaleCircleAngle;
	};

	class CircleAngleHook2
	{
		static float GetMinCircleAngle();

		// 1-6-640-0 @ 0x8824E0
		static constexpr std::uintptr_t AE_FuncID = 50648;
		static constexpr std::ptrdiff_t AE_OffsetL = 0x4C;
		static constexpr std::ptrdiff_t AE_OffsetH = 0x55;
		// 1-5-97-0 @ 0x8450A0
		static constexpr std::uintptr_t SE_FuncID = 49721;
		static constexpr std::ptrdiff_t SE_OffsetL = 0x3F;
		static constexpr std::ptrdiff_t SE_OffsetH = 0x47;

		static constexpr Patch AE_Epilog{
			"\xF3\x44\x0F\x10\xC0",  // movss xmm8, xmm0
			5
		};

		static constexpr Patch SE_Prolog{
			"\x9C"                   // pushf
			"\x48\x83\xEC\x10"       // sub rsp, 0x10
			"\xF3\x0F\x7F\x04\x24",  // push xmm0
			10
		};

		static constexpr Patch SE_Epilog{
			"\xF3\x0F\x6F\x34\x24"  // pop xmm6 < xmm0
			"\x48\x83\xC4\x10"      // add rsp, 0x10
			"\x9D"                  // popf
			"\xF3\x0F\x10\xF8",     //movss xmm7, xmm0
			14
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto handle = DKUtil::Hook::AddCaveHook(
				REL::RelocationID(SE_FuncID, AE_FuncID).address(),
				REL::Relocate(std::make_pair(SE_OffsetL, SE_OffsetH), std::make_pair(AE_OffsetL, AE_OffsetH)),
				FUNC_INFO(GetMinCircleAngle),
				DKUtil::Hook::RuntimePatch(nullptr, &SE_Prolog),
				DKUtil::Hook::RuntimePatch(&AE_Epilog, &SE_Epilog),
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};

	// TODO
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