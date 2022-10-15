#include "ConsoleCommands.h"

namespace CombatPathing
{
	bool ConsoleCommands::Exec(const RE::SCRIPT_PARAMETER*, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, double&, std::uint32_t&)
	{
		auto actor = a_thisObj ? a_thisObj->As<RE::Actor>() : nullptr;
		if (actor && a_scriptData) {
			std::optional<std::string> variableName, variableType;
			ParseParams(a_scriptData, variableName, variableType);
			if (variableName.has_value() && variableType.has_value()) {
				RE::hkbVariableValue graphVar;
				switch (variableType.value()[0]) {
				case 'b':
				case 'B':
					if (actor->GetGraphVariableBool(variableName.value(), graphVar.b))
						CPrint("\"%s\" : %d", variableName.value().c_str(), graphVar.b);
					break;
				case 'i':
				case 'I':
					if (actor->GetGraphVariableInt(variableName.value(), graphVar.i))
						CPrint("\"%s\" : %d", variableName.value().c_str(), graphVar.i);
					break;
				case 'f':
				case 'F':
					if (actor->GetGraphVariableFloat(variableName.value(), graphVar.f))
						CPrint("\"%s\" : %f", variableName.value().c_str(), graphVar.f);
					break;
				default:
					CPrint("Graph Variable Not Found!");
					break;
				}
			}
		}

		return true;
	}

	void ConsoleCommands::Register()
	{
		using Type = RE::SCRIPT_PARAM_TYPE;

		auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("DisplayGraphVariable");  // Unused
		if (info) {
			static RE::SCRIPT_PARAMETER params[] = {
				{ "String", Type::kChar, 0 },
				{ "String", Type::kChar, 0 }
			};

			info->executeFunction = Exec;
			info->SetParameters(params);
			DEBUG("Replace DisplayGraphVariable Console Command Successfully!");
		} else {
			ERROR("Fail to Locate DisplayGraphVariable Console Command!");
		}
	}

	void ConsoleCommands::CPrint(const char* a_fmt, ...)
	{
		auto console = RE::ConsoleLog::GetSingleton();
		if (console && console->IsConsoleMode()) {
			std::va_list args;
			va_start(args, a_fmt);
			console->VPrint(a_fmt, args);
			va_end(args);
		}
	}

	void ConsoleCommands::ParseParams(RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, std::optional<std::string>& a_variableName, std::optional<std::string>& a_variableType)
	{
		RE::SCRIPT_FUNCTION::StringChunk* strChunk = 0;

		if (a_scriptData->numParams < 1) {
			return;
		}

		strChunk = a_scriptData->GetStringChunk();
		a_variableName = std::make_optional(strChunk->GetString());
		if (a_scriptData->numParams < 2) {
			return;
		}

		strChunk = strChunk->GetNext()->AsString();
		a_variableType = std::make_optional(strChunk->GetString());
	}

}
