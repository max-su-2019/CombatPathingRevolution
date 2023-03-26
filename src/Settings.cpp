#include "Settings.h"
#include "Util.h"

namespace CombatPathing
{
	static auto MainConfig = COMPILE_PROXY("CombatPathingRevolution.ini"sv);

	CPRSettings::CPRSettings()
	{
		MainConfig.Bind(enableDebugLog, false);
		MainConfig.Bind<0.0, 1.0>(fallbackChanceMin, 0.0);
		MainConfig.Bind<0.0, 1.0>(fallbackChanceMax, 0.5);
		MainConfig.Bind<0.0, 1.0>(circleChanceMin, 0.0);
		MainConfig.Bind<0.0, 1.0>(circleChanceMax, 0.75);
		MainConfig.Bind<0.0, 1.0>(circleAnglePlayerMult, 0.75);

		MainConfig.Load();

		PrintSettingValue(enableDebugLog);
		PrintSettingValue(fallbackChanceMin);
		PrintSettingValue(fallbackChanceMax);
		PrintSettingValue(circleChanceMin);
		PrintSettingValue(circleChanceMax);
		PrintSettingValue(circleAnglePlayerMult);

		SetGameSettingFloat(fallbackChanceMin.get_key().data(), *fallbackChanceMin);
		SetGameSettingFloat(fallbackChanceMax.get_key().data(), *fallbackChanceMax);
		SetGameSettingFloat(circleChanceMin.get_key().data(), *circleChanceMin);
		SetGameSettingFloat(circleChanceMax.get_key().data(), *circleChanceMax);
		SetGameSettingFloat(circleAnglePlayerMult.get_key().data(), *circleAnglePlayerMult);
	}
}
