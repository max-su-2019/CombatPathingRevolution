#include "LoadGame.h"
#include "AdvanceRadius_Hook.h"
#include "Backoff_Hook.h"
#include "Circling_Hook.h"
#include "Fallback_Hook.h"

namespace CombatPathing
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostLoad) {
			AdvanceRadiusHook::Install();

			BackoffStartHook::InstallHook();
			BackoffChanceHook::InstallHook();

			CirclingChanceHook::InstallHook();
			AdvanceToCircleHook::InstallHook();
			CircleAngleHook1::InstallHook();
			CircleAngleHook2::InstallHook();
			CircleAngleHook3::InstallHook();

			FallbackDistanceHook1::InstallHook();
			FallbackDistanceHook2::InstallHook();
			FallbackWaitTimeHook1::InstallHook();
			FallbackWaitTimeHook2::InstallHook();
		}
	}

}
