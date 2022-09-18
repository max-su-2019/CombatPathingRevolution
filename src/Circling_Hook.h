#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

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
}