#include "LoadGame.h"
#include "Advance_Hook.h"
#include "Backoff_Hook.h"
#include "Circling_Hook.h"
#include "Fallback_Hook.h"
#include "PayloadInterpreter/hooks.h"
#include "Settings.h"

namespace CombatPathing
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostLoad) {
			AdvanceRadiusHook::InstallHook();
			AdvanceInterruptHook::InstallHook();

			BackoffStartHook::InstallHook();
			BackoffChanceHook::InstallHook();

			CirclingChanceHook::InstallHook();
			AdvanceToCircleHook::InstallHook();
			CircleAngleHook1::InstallHook();
			CircleAngleHook2::InstallHook();
			CircleAngleHook3::InstallHook();
			CircleViewConeHook::InstallHook();

			FallbackDistanceHook1::InstallHook();
			FallbackDistanceHook2::InstallHook();
			FallbackWaitTimeHook1::InstallHook();
			FallbackWaitTimeHook2::InstallHook();

			Hooks::hook_animationEvent::install();

		} else if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
			auto settings = CPRSettings::GetSingleton();
			if (settings->enableDebugLog) {
				spdlog::set_level(spdlog::level::debug);
				DEBUG("Enable Debug Log!");
			}
		}
	}

}
