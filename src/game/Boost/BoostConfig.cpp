#include "BoostConfig.h"
#include "Config/Config.h"

namespace boost_native
{
    bool BoostConfig::OnLoad()
    {
        enabled = sConfig.GetBoolDefault("Boost.Enable", false);
        minLevelAlliance = sConfig.GetIntDefault("Boost.MinLevelAlliance", 1);
        minLevelHorde = sConfig.GetIntDefault("Boost.MinLevelHorde", 1);
        newRaceMinLevel = sConfig.GetIntDefault("Boost.MinLevelNewRace", 1);
        starterGold = sConfig.GetIntDefault("Boost.StarterGold", 1000000);
        enableBags = sConfig.GetBoolDefault("Boost.Bags", true);
        enableBoost58 = sConfig.GetBoolDefault("Boost.Boost58", true);
        enableBoost60 = sConfig.GetBoolDefault("Boost.Boost60", true);
        enableBoost70 = sConfig.GetBoolDefault("Boost.Boost70", true);
        enableLevel58 = sConfig.GetBoolDefault("Boost.Level58", true);
        enableLevel60 = sConfig.GetBoolDefault("Boost.Level60", true);
        enableLevel70 = sConfig.GetBoolDefault("Boost.Level70", true);
        enableFoodRegs = sConfig.GetBoolDefault("Boost.FoodRegs", true);
        enablePet = sConfig.GetBoolDefault("Boost.Pet", true);
        enableAmmo = sConfig.GetBoolDefault("Boost.Ammo", true);
        enableResetTalents = sConfig.GetBoolDefault("Boost.ResetTalents", true);
        enableResetInstances = sConfig.GetBoolDefault("Boost.ResetInstances", true);
        enableUntrainPet = sConfig.GetBoolDefault("Boost.UntrainPet", true);
        enableMounts = sConfig.GetBoolDefault("Boost.Mounts", true);
        enableTaxi = sConfig.GetBoolDefault("Boost.Taxi", true);
        enableFirstAid = sConfig.GetBoolDefault("Boost.FirstAid", true);
        enableSpells = sConfig.GetBoolDefault("Boost.Spells", true);
        enableGearBlue58 = sConfig.GetBoolDefault("Boost.GearBlue58", true);
        enableBoost60Bis = sConfig.GetBoolDefault("Boost.Level60Bis", true);
        enableBoost70Bis = sConfig.GetBoolDefault("Boost.Level70Bis", true);
        enableTeleports = sConfig.GetBoolDefault("Boost.Teleports", true);
        return true;
    }
}