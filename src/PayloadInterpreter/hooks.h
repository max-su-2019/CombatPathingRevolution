#pragma once
#include "payloadManager.h"

namespace Hooks
{
	class hook_animationEvent
	{
	public:
		static void install()
		{
			INFO("Installing animation event hook...");
			REL::Relocation<uintptr_t> AnimEventVtbl_NPC{ RE::VTABLE_Character[2] };

			_ProcessEvent_NPC = AnimEventVtbl_NPC.write_vfunc(0x1, ProcessEvent_NPC);

			INFO("...animation event hook installed");
		}

	private:
		static inline void ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
		{
			if (a_event->tag != "CPR") {
				return;
			}
			RE::TESObjectREFR* holder = const_cast<RE::TESObjectREFR*>(a_event->holder);

			if (!holder) {
				return;
			}
			std::string payload = std::string(a_event->payload.data());

			CPR::delegateNative(holder->As<RE::Actor>(), &payload);
		}

		using EventResult = RE::BSEventNotifyControl;
		static EventResult ProcessEvent_NPC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
		{
			ProcessEvent(a_sink, a_event, a_eventSource);
			return _ProcessEvent_NPC(a_sink, a_event, a_eventSource);
		}

		static inline REL::Relocation<decltype(ProcessEvent_NPC)> _ProcessEvent_NPC;
	};

	void install()
	{
		hook_animationEvent::install();
	}

}
