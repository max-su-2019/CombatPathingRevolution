#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class FallbackDistanceHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(46712) };  //sub_1407D73D0
			_GetFallbackDistance = trampoline.write_call<5>(Base.address() + 0x1EB, GetFallbackDistance);
			INFO("Hook FallbackStart!");
		}

	private:
		static float GetFallbackDistance(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackDistance)> _GetFallbackDistance;
	};

	class FallbackDistanceHook2
	{
		static float GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor* a_he);

		static constexpr std::uintptr_t FuncID = 46713;   //1407d7740
		static constexpr std::ptrdiff_t OffsetL = 0x246;  //1407D7986
		static constexpr std::ptrdiff_t OffsetH = 0x24E;  //1407D798E

		static constexpr Patch RelocateReturn{
			// addss xmm6, xmm0
			"\xF3\x0F\x58\xF0",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();
			Patch RelocatePointer{
				AsPointer(funcAddr + OffsetL + 0x10),  //7D7996
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(GetMaxFallbackDistance),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("Hook FallbackDistanceUpdate");
		}
	};

	class FallbackWaitTimeHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(46713) };  //sub_1407D7740
			_GetFallbackWaitTime = trampoline.write_call<5>(Base.address() + 0xEA, GetFallbackWaitTime);
			INFO("Hook FallbackStart!");
		}

	private:
		static float GetFallbackWaitTime(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackWaitTime)> _GetFallbackWaitTime;
	};

	class FallbackWaitTimeHook2
	{
		static float GetMinFallbackWaitTime(RE::Actor* a_me, RE::Actor* a_he);

		static constexpr std::uintptr_t FuncID = 46713;   //1407d7740
		static constexpr std::ptrdiff_t OffsetL = 0x2C0;  //1407D7A00
		static constexpr std::ptrdiff_t OffsetH = 0x2C8;  //1407D7A08

		static constexpr Patch RelocateReturn{
			// movss xmm1, xmm0
			"\xF3\x0F\x10\xC8",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();
			Patch RelocatePointer{
				AsPointer(funcAddr + 0x256),  //7D7996
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(GetMinFallbackWaitTime),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("Hook FallbackWaitTimeUpdate");
		}
	};
}