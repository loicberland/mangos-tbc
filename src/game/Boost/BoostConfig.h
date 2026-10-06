#pragma once
#include "Common.h"

namespace boost_native
{
    class BoostConfig
    {
    public:
        BoostConfig() = default;
        bool OnLoad();

    public:
        bool enabled;
        uint32 newRaceMinLevel;
        uint32 minLevelAlliance;
        uint32 minLevelHorde;
        uint32 starterGold;
        bool enableBoost58;
        bool enableBoost60;
        bool enableBoost70;
        bool enableLevel58;
        bool enableLevel60;
        bool enableLevel70;
        bool enableTaxi;
        bool enableBags;
        bool enableFirstAid;
        bool enableSpells;
        bool enableMounts;
        bool enablePet;
        bool enableAmmo;
        bool enableResetTalents;
        bool enableResetInstances;
        bool enableUntrainPet;
        bool enableFoodRegs;
        bool enableGearBlue58;
        bool enableBoost60Bis;
        bool enableBoost70Bis;
        bool enableTeleports;
    };
}