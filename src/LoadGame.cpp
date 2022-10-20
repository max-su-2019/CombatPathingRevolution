#include "LoadGame.h"
#include "AdvanceRadius_Hook.h"
#include "Backoff_Hook.h"
#include "Circling_Hook.h"
#include "ConsoleCommands.h"
#include "Fallback_Hook.h"

namespace CombatPathing
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostLoad) {
			ConsoleCommands::Register();

			AdvanceRadiusHook::Install();

			BackoffStartHook::InstallHook();
			BackoffChanceHook::InstallHook();

			CirclingHook::InstallHook();

			FallbackStartHook::InstallHook();
			FallbackUpdateHook::InstallHook();

			//Hook_GetOffensive::install();
		}
	}

}
