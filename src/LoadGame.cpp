#include "LoadGame.h"
#include "Circling_Hook.h"
#include "CombatRadius_Hook.h"
#include "Fallback_Hook.h"

namespace CombatPathing
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostLoad) {
			CirclingHook::InstallHook();
			CombatRadiusHook::Install();
			FallbackStartHook::InstallHook();
			FallbackUpdateHook::InstallHook();
		}
	}

}
