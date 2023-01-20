#pragma once
#include "DKUtil/Config.hpp"
#include "DKUtil/Utility.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class CPRSettings : public DKUtil::model::Singleton<CPRSettings>
	{
		friend DKUtil::model::Singleton<CPRSettings>;

	private:
		CPRSettings();

		template <class T>
		static void PrintSettingValue(const T& a_setting)
		{
			INFO("Setting:\"{}\" is {}"sv, a_setting.get_key(), a_setting.get_data());
		}

		Double fallbackChanceMin{ "fCombatFallbackChanceMin", "GameSettings" };
		Double fallbackChanceMax{ "fCombatFallbackChanceMax", "GameSettings" };
		Double circleChanceMin{ "fCombatCircleChanceMin", "GameSettings" };
		Double circleChanceMax{ "fCombatCircleChanceMax", "GameSettings" };
		Double circleAnglePlayerMult{ "fCombatCircleAnglePlayerMult", "GameSettings" };

	public:
		Boolean enableDebugLog{ "EnableDebugLog", "Debug" };
	};
}