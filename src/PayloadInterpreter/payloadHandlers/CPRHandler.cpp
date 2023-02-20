#include "RE/CombatBehaviorNodesMovement.h"
#include "payloadHandler.h"

void CPRHandler::process(RE::Actor* actor, std::vector<std::string_view>* v, FUNCTION f)
{
	switch (f) {
	case FUNCTION::EnableAdvance:
		enableAdvance(actor, v);
		break;
	case FUNCTION::EnableBackoff:
		enableBackoff(actor, v);
		break;
	case FUNCTION::EnableCircling:
		enableCircling(actor, v);
		break;
	case FUNCTION::EnableSurround:
		enableSurround(actor, v);
		break;
	case FUNCTION::EnableFallback:
		enableFallback(actor, v);
		break;
	case FUNCTION::DisableAll:
		disableAll(actor);
		break;
	}
}

static void SetCPRVariables(RE::Actor* a_actor, const std::string actionName, const std::vector<std::string>& paramNames, std::vector<std::string_view>* v)
{
	DEBUG("Enable {} in actor :{}-{:x}", actionName, a_actor->GetName(), a_actor->GetFormID());
	a_actor->SetGraphVariableBool(actionName, true);

	for (int i = 0; i < paramNames.size(); i++) {
		const auto argIndex = i + 1;
		float value;
		if (argIndex < v->size() && Utils::string_view::to_float(v->at(argIndex), value)) {
			a_actor->SetGraphVariableFloat(paramNames[i], value);
		} else {
			DEBUG("Fail to parse argument \"{}\" for {} in actor :{}-{:x}", paramNames[i], actionName, a_actor->GetName(), a_actor->GetFormID());
			return;
		}
	}
}

void CPRHandler::enableAdvance(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	static const std::string actionName = "CPR_EnableAdvanceRadius";

	static const std::vector<std::string> paramName = {
		"CPR_InnerRadiusMin",
		"CPR_InnerRadiusMid",
		"CPR_InnerRadiusMax",
		"CPR_OuterRadiusMin",
		"CPR_OuterRadiusMid",
		"CPR_OuterRadiusMax"
	};

	SetCPRVariables(a_actor, actionName, paramName, v);

	if (InterruptActiveAction<RE::NodeCloseMovementAdvance>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementAdvance in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
}

void CPRHandler::enableBackoff(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	static const std::string actionName = "CPR_EnableBackoff";

	static const std::vector<std::string> paramNames = {
		"CPR_BackoffMinDistMult",
		"CPR_BackoffChance"
	};

	SetCPRVariables(a_actor, actionName, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementBackoff>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementBackoff in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::enableCircling(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	static const std::string actionName = "CPR_EnableCircling";

	static const std::vector<std::string> paramNames = {
		"CPR_CirclingDistMin",
		"CPR_CirclingDistMax",
		"CPR_CirclingAngleMin",
		"CPR_CirclingAngleMax",
		"CPR_CirclingViewConeAngle"
	};

	SetCPRVariables(a_actor, actionName, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementCircle>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementCircle in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::enableSurround(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	static const std::string actionName = "CPR_EnableSurround";

	static const std::vector<std::string> paramNames = {
		"CPR_SurroundDistMin",
		"CPR_SurroundDistMax"
	};

	SetCPRVariables(a_actor, actionName, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementSurround>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementSurround in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::enableFallback(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	static const std::string actionName = "CPR_EnableFallback";

	static const std::vector<std::string> paramNames = {
		"CPR_FallbackDistMin",
		"CPR_FallbackDistMax",
		"CPR_FallbackWaitTimeMin",
		"CPR_FallbackWaitTimeMax",
	};

	SetCPRVariables(a_actor, actionName, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementFallback>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementFallback in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::disableAll(RE::Actor* a_actor)
{
	DEBUG("CPR:DisableAdvance for {} - {:x}", a_actor->GetName(), a_actor->formID);
	a_actor->SetGraphVariableBool("CPR_EnableAdvanceRadius", false);

	DEBUG("CPR:DisableBackoff for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Backoff data.
	a_actor->SetGraphVariableBool("CPR_EnableBackoff", false);

	DEBUG("CPR:DisableCircling for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Circling data.
	a_actor->SetGraphVariableBool("CPR_EnableCircling", false);

	DEBUG("CPR:DisableSurround for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Surround data.
	a_actor->SetGraphVariableBool("CPR_EnableSurround", false);

	DEBUG("CPR:DisableFallback for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Fallback data.
	a_actor->SetGraphVariableBool("CPR_EnableFallback", false);
}
