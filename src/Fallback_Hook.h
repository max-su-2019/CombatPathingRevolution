#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class Hook_GetOffensive
	{
	public:
		static void install()
		{
			auto& trampoline = SKSE::GetTrampoline();
			REL::Relocation<std::uintptr_t> Base{ REL::ID(49747) };
			_GetOffensive = trampoline.write_call<5>(Base.address() + 0x15, GetOffensive);
			INFO("hook:GetOffensive");
		}

	private:
		static float GetOffensive(RE::Actor* a_actor)
		{
			return -1.0f;
		}

		static inline REL::Relocation<decltype(GetOffensive)> _GetOffensive;
	};

	class FallbackStartHook
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

	class FallbackUpdateHook
	{
		static float GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor* a_he);

		static constexpr std::uintptr_t FuncID = 46713;
		static constexpr std::ptrdiff_t OffsetL = 0x246;  //7D7986
		static constexpr std::ptrdiff_t OffsetH = 0x24E;  //7D798E

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

			INFO("Hook FallbackUpdate");
		}
	};
}