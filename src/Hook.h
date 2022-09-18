#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class CombatRadiusHook
	{
		// 1-6-353-0 @ 0x870CF0
		static inline constexpr std::uint64_t AE_FuncID = 50643;
		// 1-5-97-0 @ 0x844E40
		static inline constexpr std::uint64_t SE_FuncID = 49716;

		static inline constexpr std::ptrdiff_t RadiusL = 0x99;
		static inline constexpr std::ptrdiff_t RadiusH = 0xA0;
		static inline constexpr std::ptrdiff_t MedianL = 0x139;
		static inline constexpr std::ptrdiff_t MedianH = 0x145;

		static inline constexpr Patch RadiusPatch{
			// lahf
			"\x9F\x50"
			// mov rdx, rdi
			"\x48\x89\xFA"
			// movss xmm2, xmm6
			"\xF3\x0F\x10\xD6"
			// mov r9, rbp
			"\x49\x89\xE9"
			// mov [rsp-0x8], rbx
			"\x48\x89\x5C\x24\xF8",
			17
		};

		static inline constexpr Patch MedianPatch{
			// xor cl
			"\x30\xC9",
			19
		};

		static inline constexpr Patch Epilog{
			// sahf
			"\x58\x9E",
			2
		};

		static float RescaleRadius(float a_delta, float min, float mid, float max);

		// cl, rdx, xmm2, r9, rsp-0x8
		static void RecalculateCombatRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker);

	public:
		static void Install()
		{
			SKSE::AllocTrampoline(static_cast<size_t>(1) << 7);

			auto Hook_SetRadius = DKUtil::Hook::AddCaveHook(
				DKUtil::Hook::IDToAbs(50643, 49716),
				{ RadiusL, RadiusH },
				FUNC_INFO(RecalculateCombatRadius),
				&RadiusPatch,
				&Epilog,
				DKUtil::Hook::CaveHookFlag::kRestoreBeforeProlog);

			Hook_SetRadius->Enable();

			auto Hook_SetMedian = DKUtil::Hook::AddCaveHook(
				DKUtil::Hook::IDToAbs(50643, 49716),
				{ MedianL, MedianH },
				FUNC_INFO(RecalculateCombatRadius),
				&MedianPatch,
				&Epilog,
				DKUtil::Hook::CaveHookFlag::kRestoreBeforeProlog);

			// recalculate displacement
			Disp32 disp = *std::bit_cast<Disp32*>(AsPointer(Hook_SetMedian->TramEntry + 0x4));
			DKUtil::Hook::WriteImm(Hook_SetMedian->TramEntry + 0x4, static_cast<Disp32>(Hook_SetMedian->CaveEntry + disp - Hook_SetMedian->TramEntry));

			DKUtil::Hook::WritePatch(Hook_SetMedian->TramEntry + Hook_SetMedian->CaveSize + 0x2, &RadiusPatch);

			Hook_SetMedian->Enable();

			INFO("Hook SetCombatRadius!");
		}
	};

	class CirclingHook
	{
		static constexpr Patch RelocatePointer{
			"\x4C\x8B\x4C\x24\xB8"
			"\xE9\x89\xFC\xFF\xFF",
			10
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(49720, 50647) };
			const auto _originalFunc = *std::bit_cast<std::uintptr_t*>(Base.address() + 0x23);

			auto handle = DKUtil::Hook::AddASMPatch(Base.address(), { 0x22, 0x2C }, &RelocatePointer);
			// recalculate displacement
			DKUtil::Hook::WriteImm(handle->TramEntry + 0x6, _originalFunc + 5);
			handle->Enable();

			_GetCirclingChance = trampoline.write_branch<5>(Base.address() + 0x27, GetCirclingChance);
			INFO("Hook Circling!");
		}

	private:
		static float GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance, [[maybe_unused]] RE::Character* a_actor = nullptr);

		static inline REL::Relocation<decltype(GetCirclingChance)> _GetCirclingChance;
	};

	class FallbackHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(46712) };  //sub_1407D73D0
			_GetFallbackDistance = trampoline.write_call<5>(Base.address() + 0x1EB, GetFallbackDistance);
			INFO("Hook GetFallbackDistance!");
		}

	private:
		static float GetFallbackDistance(RE::Actor* a_actor);

		static float RescaleDistance(float a_mult, float a_min, float a_max);

		static inline REL::Relocation<decltype(GetFallbackDistance)> _GetFallbackDistance;
	};

	class FallbackHook2
	{
		static float RecalculateFallbackDistance(RE::Character* a_me, RE::Character* a_he)
		{
			if (!a_me || !a_he) {
				return 0.f;
			}

			DEBUG("ME: {}|0x{:08X} <-> HE: {}|0x{:08X}", a_me->GetName(), a_me->GetFormID(), a_he->GetName(), a_he->GetFormID());

			return 1000.f;
		}

		static constexpr std::uintptr_t FuncID = 46713;
		static constexpr std::ptrdiff_t OffsetL = 0x246;
		static constexpr std::ptrdiff_t OffsetH = 0x24E;

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
				AsPointer(funcAddr + OffsetL + 0x10),
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(RecalculateFallbackDistance),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::CaveHookFlag::kSkipOP);

			handle->Enable();

			INFO("Installed FallbackHook2");
		}
	};

}