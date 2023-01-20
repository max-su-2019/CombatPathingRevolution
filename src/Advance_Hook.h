#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class AdvanceRadiusHook
	{
		// 1-6-640-0 @ 0x882280
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

		// cl, rdx, xmm2, r9, rsp-0x8
		static void RecalculateAdvanceRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker);

	public:
		static void Install()
		{
			SKSE::AllocTrampoline(static_cast<size_t>(1) << 7);

			auto Hook_SetRadius = DKUtil::Hook::AddCaveHook(
				DKUtil::Hook::IDToAbs(50643, 49716),
				{ RadiusL, RadiusH },
				FUNC_INFO(RecalculateAdvanceRadius),
				&RadiusPatch,
				&Epilog,
				DKUtil::Hook::HookFlag::kRestoreBeforeProlog);

			Hook_SetRadius->Enable();

			auto Hook_SetMedian = DKUtil::Hook::AddCaveHook(
				DKUtil::Hook::IDToAbs(50643, 49716),
				{ MedianL, MedianH },
				FUNC_INFO(RecalculateAdvanceRadius),
				&MedianPatch,
				&Epilog,
				DKUtil::Hook::HookFlag::kRestoreBeforeProlog);

			// recalculate displacement
			Disp32 disp = *std::bit_cast<Disp32*>(AsPointer(Hook_SetMedian->TramEntry + 0x4));
			DKUtil::Hook::WriteImm(Hook_SetMedian->TramEntry + 0x4, static_cast<Disp32>(Hook_SetMedian->CaveEntry + disp - Hook_SetMedian->TramEntry));

			DKUtil::Hook::WritePatch(Hook_SetMedian->TramEntry + Hook_SetMedian->CaveSize + 0x2, &RadiusPatch);

			Hook_SetMedian->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};

	class AdvanceInterruptHook
	{
	public:
		static void InstallHook()
		{
			auto& trampoline = SKSE::GetTrampoline();
			_Update = trampoline.write_branch<5>(REL::ID(529964).address() + 0x9, Update);  // SkyrimSE.exe+7DFF29
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static void Update(char** context);
		static inline REL::Relocation<decltype(Update)> _Update;
	};
}