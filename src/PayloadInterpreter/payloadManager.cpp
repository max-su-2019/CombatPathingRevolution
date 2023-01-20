#include "payloadManager.h"

constexpr uint32_t hash(const char* data, size_t const size) noexcept
{
	uint32_t hash = 5381;

	for (const char* c = data; c < data + size; ++c) {
		hash = ((hash << 5) + hash) + (unsigned char)*c;
	}

	return hash;
}

constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
{
	return hash(str, size);
}

void CPR::delegateNative(RE::Actor* actor, std::string* a_payload)
{
	DEBUG("CPR instruction triggered for {}; instruction: {}", actor->GetName(), a_payload->c_str());
	std::vector<std::string_view> tokens;
	Utils::splitSV(tokens, std::string_view(a_payload->c_str()), '|');
	switch (hash(tokens[0].data(), tokens[0].size())) {
	case "EnableAdvance"_h:  // CPR.EnableAdvance|111|222|333|444|555|666
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableAdvance);
		break;
	case "EnableBackoff"_h:  // CPR.EnableBackoff|11|22
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableBackoff);
		break;
	case "EnableCircling"_h:  // CPR.EnableCircling|33|44
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableCircling);
		break;
	case "EnableSurround"_h:  // CPR.EnableSurround|...
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableSurround);
		break;
	case "EnableFallback"_h:  // CPR.EnableFallback|...
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableFallback);
		break;
	case "DisableAll"_h:  // CPR.DisableAll
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::DisableAll);
		break;
	}
}
