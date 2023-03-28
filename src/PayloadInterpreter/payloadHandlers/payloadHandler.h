#pragma once
#include "PayloadInterpreter/Dtry_Utils.h"
#include "RE/CombatBehaviorController.h"
#include "RE/CombatBehaviorNodesMovement.h"

class payloadHandler
{
public:
	/*process the parameters of a payload funcion.*/
	static void process(RE::Actor* actor, std::vector<std::string_view>* param){
		//interpret the information in the payload command and execute commands.
	};

	/*Prints out an error message and the command errored on.
	@param v: tokenized instruction.
	@param errMsg: Error message to print.*/
	static void printErrMsg(std::vector<std::string_view>* v, std::string errMsg)
	{
		const char* instruction = v->at(0).data();  //char* at the start of the instruction, printing from this point will print out the whole instruction.
		WARN("Error: {} Errored instruction: {}", errMsg, instruction);
	}
	/*check if the amount of parameter in the vector is correct, ignoring the instruction. If not, prints an error message.
	@param param: vector containing all parameters and instruction.
	@param ct: expected i.e. correct number of parameters.
	@return whether the number of tokens in param matches ct.*/
	static bool checkParamCt(std::vector<std::string_view>* param, int ct)
	{
		if (param->size() != ct + 1) {
			printErrMsg(param,
				"incorrect number of parameters passed in, expected: " + std::to_string(ct) + " received: " + std::to_string(param->size() - 1));
			return false;
		}
		return true;
	}
};

/*MaxSu's CPR stuff*/
class CPRHandler : public payloadHandler
{
public:
	enum FUNCTION
	{
		EnableAdvance,
		EnableBackoff,
		EnableCircling,
		EnableSurround,
		EnableFallback,
		DisableAll
	};
	static void process(RE::Actor* actor, std::vector<std::string_view>* v, FUNCTION f);

private:
	static void enableAdvance(RE::Actor* actor, std::vector<std::string_view>* v);
	static void enableBackoff(RE::Actor* actor, std::vector<std::string_view>* v);
	static void enableCircling(RE::Actor* actor, std::vector<std::string_view>* v);
	static void enableSurround(RE::Actor* actor, std::vector<std::string_view>* v);
	static void enableFallback(RE::Actor* actor, std::vector<std::string_view>* v);
	static void disableAll(RE::Actor* actor);

	template <typename T>
	static bool InterruptActiveAction(RE::Actor* a_actor)
	{
		using NodeState = RE::CombatBehaviorTreeControl::Control_AI_States;

		if (a_actor) {
			auto combatCtrl = a_actor->GetActorRuntimeData().combatController;
			auto behaviorCtrl = combatCtrl ? combatCtrl->behaviorController : nullptr;
			if (behaviorCtrl) {
				for (auto nodeCtrl : behaviorCtrl->controls_08) {
					if (nodeCtrl && nodeCtrl->cur_node && nodeCtrl->current_state == NodeState::processing) {
						auto activeNode = skyrim_cast<T*>(nodeCtrl->cur_node);
						if (activeNode) {
							return a_actor->SetGraphVariableBool("CPR_InterruptAction", true);
						}
					}
				}
			}
		}

		return false;
	}
};
