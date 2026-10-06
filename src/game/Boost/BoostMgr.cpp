#include "BoostMgr.h"

#include "AI/ScriptDevAI/include/sc_gossip.h"
#include "Entities/GossipDef.h"
#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Globals/ObjectMgr.h"
#include "Spells/SpellMgr.h"
#include "Grids/GridNotifiers.h"
#include "Tools/Language.h"
#include "AI/BaseAI/CreatureAI.h"
#include "Entities/Creature.h"
#include "Entities/Pet.h"
#include "Maps/Map.h"
#include "Server/WorldSession.h"

#ifdef ENABLE_PLAYERBOTS
#include "playerbot/PlayerbotAI.h"
#endif

#define BOOST_EXPANSION 1

#if BOOST_EXPANSION == 0
#define MOUNT_LEVEL 40
#endif
#if BOOST_EXPANSION == 1
#define MOUNT_LEVEL 30
#endif
#if BOOST_EXPANSION == 2
#define MOUNT_LEVEL 20
#endif

namespace boost_native
{
    BoostMgr& BoostMgr::Instance()
    {
        static BoostMgr instance;
        return instance;
    }

    const BoostConfig* BoostMgr::GetConfig() const
    {
        return &m_config;
    }

    void BoostMgr::OnInitialize()
    {
        m_config.OnLoad();
        FullGearList.clear();
        FullGearList.insert(std::end(FullGearList), std::begin(PlatePrimaryGear), std::end(PlatePrimaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(LeatherPrimaryGear), std::end(LeatherPrimaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(ClothPrimaryGear), std::end(ClothPrimaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(MailPrimaryGear), std::end(MailPrimaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(SharedSecondaryGear), std::end(SharedSecondaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(PlateSecondaryGear), std::end(PlateSecondaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(ClothSecondaryGear), std::end(ClothSecondaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(LeatherSecondaryGear), std::end(LeatherSecondaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(MailSecondaryGear), std::end(MailSecondaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(ProtWarriorTertiaryGear), std::end(ProtWarriorTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(FuryWarriorTertiaryGear), std::end(FuryWarriorTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(ArmsWarriorTertiaryGear), std::end(ArmsWarriorTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(HolyPaladinTertiaryGear), std::end(HolyPaladinTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(RetPaladinTertiaryGear), std::end(RetPaladinTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(ProtPaladinTertiaryGear), std::end(ProtPaladinTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(BalanceDruidTertiaryGear), std::end(BalanceDruidTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(FeralDruidTertiaryGear), std::end(FeralDruidTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(RestoDruidTertiaryGear), std::end(RestoDruidTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(DiscHolyPriestTertiaryGear), std::end(DiscHolyPriestTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(CasterDPSTertiaryGear), std::end(CasterDPSTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(EnhanceShamanTertiaryGear), std::end(EnhanceShamanTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(ElementalShamanTertiaryGear), std::end(ElementalShamanTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(RestoShamanTertiaryGear), std::end(RestoShamanTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(HunterTertiaryGear), std::end(HunterTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(AssassinRogueTertiaryGear), std::end(AssassinRogueTertiaryGear));
        FullGearList.insert(std::end(FullGearList), std::begin(CombatSubRogueTertiaryGear), std::end(CombatSubRogueTertiaryGear));

        FullGearListInstant58.clear();
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58SharedPhysical), std::end(Instant58SharedPhysical));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Bow), std::end(Instant58Bow));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58LeatherOffpieces), std::end(Instant58LeatherOffpieces));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Staff), std::end(Instant58Staff));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58ClothCaster), std::end(Instant58ClothCaster));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58MailOffpieces), std::end(Instant58MailOffpieces));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58GavelAndShield), std::end(Instant58GavelAndShield));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58DualWield), std::end(Instant58DualWield));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58PlateOffpieces), std::end(Instant58PlateOffpieces));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Hunter), std::end(Instant58Hunter));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Rogue), std::end(Instant58Rogue));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Druid), std::end(Instant58Druid));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Mage), std::end(Instant58Mage));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Paladin), std::end(Instant58Paladin));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Priest), std::end(Instant58Priest));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Shaman), std::end(Instant58Shaman));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Warlock), std::end(Instant58Warlock));
        FullGearListInstant58.insert(std::end(FullGearListInstant58), std::begin(Instant58Warrior), std::end(Instant58Warrior));

        FullGearListBiS60.clear();
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_ShamanResto), std::end(Lvl60BiS_ShamanResto));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_ShamanEnhancement), std::end(Lvl60BiS_ShamanEnhancement));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_ShamanElemental), std::end(Lvl60BiS_ShamanElemental));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_PriestShadow), std::end(Lvl60BiS_PriestShadow));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_PriestDiscHoly), std::end(Lvl60BiS_PriestDiscHoly));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_PaladinHoly), std::end(Lvl60BiS_PaladinHoly));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_PaladinRetribution), std::end(Lvl60BiS_PaladinRetribution));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_PaladinProtection), std::end(Lvl60BiS_PaladinProtection));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_WarriorFuryArms), std::end(Lvl60BiS_WarriorFuryArms));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_WarriorProtection), std::end(Lvl60BiS_WarriorProtection));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_DruidFeralCat), std::end(Lvl60BiS_DruidFeralCat));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_DruidFeralBear), std::end(Lvl60BiS_DruidFeralBear));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_DruidBalance), std::end(Lvl60BiS_DruidBalance));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_DruidResto), std::end(Lvl60BiS_DruidResto));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_Rogue), std::end(Lvl60BiS_Rogue));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_Mage), std::end(Lvl60BiS_Mage));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_Hunter), std::end(Lvl60BiS_Hunter));
        FullGearListBiS60.insert(std::end(FullGearListBiS60), std::begin(Lvl60BiS_Warlock), std::end(Lvl60BiS_Warlock));

        FullGearListBiS70.clear();
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_ShamanResto), std::end(Lvl70BiS_ShamanResto));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_ShamanEnhancement), std::end(Lvl70BiS_ShamanEnhancement));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_ShamanElemental), std::end(Lvl70BiS_ShamanElemental));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_PriestShadow), std::end(Lvl70BiS_PriestShadow));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_PriestDiscHoly), std::end(Lvl70BiS_PriestDiscHoly));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_PaladinHoly), std::end(Lvl70BiS_PaladinHoly));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_PaladinRetribution), std::end(Lvl70BiS_PaladinRetribution));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_PaladinProtection), std::end(Lvl70BiS_PaladinProtection));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_WarriorFuryArms), std::end(Lvl70BiS_WarriorFuryArms));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_WarriorProtection), std::end(Lvl70BiS_WarriorProtection));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_DruidFeralCat), std::end(Lvl70BiS_DruidFeralCat));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_DruidFeralBear), std::end(Lvl70BiS_DruidFeralBear));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_DruidBalance), std::end(Lvl70BiS_DruidBalance));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_DruidResto), std::end(Lvl70BiS_DruidResto));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_Rogue), std::end(Lvl70BiS_Rogue));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_Mage), std::end(Lvl70BiS_Mage));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_Hunter), std::end(Lvl70BiS_Hunter));
        FullGearListBiS70.insert(std::end(FullGearListBiS70), std::begin(Lvl70BiS_Warlock), std::end(Lvl70BiS_Warlock));
    }

    uint32 BoostMgr::GetStarterMountForRace(Player* player)
    {
        uint32 mountEntry = 0;
        switch (player->getRace())
        {
        case RACE_HUMAN: mountEntry = 2411; break;      // Black Stallion Bridle
        case RACE_DWARF: mountEntry = 5872; break;      // Brown Ram
        case RACE_NIGHTELF: mountEntry = 8632; break;   // Reins of the Spotted Frostsaber
        case RACE_GNOME: mountEntry = 8595; break;      // Blue Mechanostrider
        case RACE_ORC: mountEntry = 5665; break;        // Horn of the Dire Wolf
        case RACE_UNDEAD: mountEntry = 13331; break;    // Red Skeletal Horse
        case RACE_TAUREN: mountEntry = 15277; break;    // Gray Kodo
        case RACE_TROLL: mountEntry = 8588; break;      // Whistle of the Emerald Raptor
#if BOOST_EXPANSION > 0
        case RACE_DRAENEI: mountEntry = 29744; break;   // Gray Elekk
        case RACE_BLOODELF: mountEntry = 29221; break;  // Black Hawkstrider
#endif
        }
        return mountEntry;
    }

    uint32 BoostMgr::GetStarterEpicMountForRace(Player* player)
    {
        uint32 mountEntry = 0;
        switch (player->getRace())
        {
        case RACE_HUMAN: mountEntry = 18776; break;     // Swift Palomino
        case RACE_DWARF: mountEntry = 18787; break;     // Swift Gray Ram
        case RACE_NIGHTELF: mountEntry = 18767; break;  // Reins of the Swift Mistsaber
        case RACE_GNOME: mountEntry = 18772; break;     // Swift Green Mechanostrider
        case RACE_ORC: mountEntry = 18797; break;       // Horn of the Swift Timber Wolf 
        case RACE_UNDEAD: mountEntry = 18791; break;    // Purple Skeletal Warhorse
        case RACE_TAUREN: mountEntry = 18795; break;    // Great Gray Kodo
        case RACE_TROLL: mountEntry = 18790; break;     // Swift Orange Raptor
#if BOOST_EXPANSION > 0
        case RACE_DRAENEI: mountEntry = 29745; break;   // Great Blue Elekk
        case RACE_BLOODELF: mountEntry = 28927; break;  // Red Hawkstrider
#endif
        }
        return mountEntry;
    }

    bool BoostMgr::OnPreGossipHello(Player* player, Creature* creature)
    {
        if (GetConfig()->enabled)
        {
            if (player && creature)
            {
                // Check if speaking with boost npc
                if (creature->GetEntry() != NPC_BOOSTER)
                    return false;

#ifdef ENABLE_PLAYERBOTS
                if (sRandomPlayerbotMgr.IsFreeBot(player))
                    return false;
#endif

                player->GetPlayerMenu()->ClearMenus();

                uint32 minLevel = 1;
                if (player->GetTeam() == ALLIANCE)
                    minLevel = GetConfig()->minLevelAlliance;
                else if (player->GetTeam() == HORDE)
                    minLevel = GetConfig()->minLevelHorde;
#if BOOST_EXPANSION > 0
                if (player->getRace() == RACE_DRAENEI || player->getRace() == RACE_BLOODELF)
                    minLevel = GetConfig()->newRaceMinLevel;
#endif

                if (player->GetLevel() < minLevel)
                {
                    player->SEND_GOSSIP_MENU(GOSSIP_BOOST_LOW_LEVEL, creature->GetObjectGuid());
                    return true;
                }

                if (GetConfig()->enableBoost58 && player->GetLevel() < 58)
                {
#if BOOST_EXPANSION > 0
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Boost to level 58. |cFF006400[Teleport to Dark Portal]|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30, "Are you sure?", 0, false);
#else
                    if (player->GetTeam() == ALLIANCE)
                        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1,
                                                         "Boost to level 58. |cFF006400[Teleport to Stormwind City]|r",
                                                         GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30,
                                                         "Are you sure?", 0, false);
                    else
                        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1,
                                                         "Boost to level 58. |cFF006400[Teleport to Orgrimmar]|r",
                                                         GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30,
                                                         "Are you sure?", 0, false);
#endif
                }
                /*if (player->GetLevel() < 60)
                {
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Boost to level 60. |cFF006400[Teleport to Dark Portal]|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 40, "Are you sure?", 0, false);
                }
                if (player->GetLevel() < 70)
                {
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Boost to level 70. |cFF006400[Teleport to Shattrath]|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50, "Are you sure?", 0, false);
                }*/

                if (GetConfig()->enableLevel58 && player->GetLevel() < 58)
                {
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Give level 58 |cFF006400[Level and basic skills]|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, "Are you sure?", 0, false);
                }
                if (GetConfig()->enableLevel60 && player->GetLevel() < 60)
                {
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Give level 60 |cFF006400[Level and basic skills]|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "Are you sure?", 0, false);
                }
#if BOOST_EXPANSION > 0
                if (GetConfig()->enableLevel70 && player->GetLevel() < 70)
                {
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Give level 70 |cFF006400[Level and basic skills]|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5, "Are you sure?", 0, false);
                }
#endif

                if (GetConfig()->enableTeleports && player->GetLevel() >= 58)
                {
#if BOOST_EXPANSION > 0
                    if (player->GetAreaId() != 3703)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Shattrath City", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    if (player->GetAreaId() != 72)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Blasted Lands (Dark Portal)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
#else
                    if (player->GetTeam() == ALLIANCE && player->GetAreaId() != 1519)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Stormwind City", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    }
                    else if (player->GetTeam() == HORDE && player->GetAreaId() != 1637)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Orgrimmar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    }
#endif

                    CustomTeleportLocation startLocation;
                    switch (player->getRace())
                    {
                    case RACE_HUMAN:
                        startLocation = { teleLocs[4].map, teleLocs[4].x, teleLocs[4].y, teleLocs[4].z, teleLocs[4].o };
                        break;
                    case RACE_DWARF:
                    case RACE_GNOME:
                        startLocation = { teleLocs[5].map, teleLocs[5].x, teleLocs[5].y, teleLocs[5].z, teleLocs[5].o };
                        break;
                    case RACE_NIGHTELF:
                        startLocation = { teleLocs[6].map, teleLocs[6].x, teleLocs[6].y, teleLocs[6].z, teleLocs[6].o };
                        break;
                    case RACE_TROLL:
                    case RACE_ORC:
                        startLocation = { teleLocs[8].map, teleLocs[8].x, teleLocs[8].y, teleLocs[8].z, teleLocs[8].o };
                        break;
                    case RACE_UNDEAD:
                        startLocation = { teleLocs[9].map, teleLocs[9].x, teleLocs[9].y, teleLocs[9].z, teleLocs[9].o };
                        break;
                    case RACE_TAUREN:
                        startLocation = { teleLocs[10].map, teleLocs[10].x, teleLocs[10].y, teleLocs[10].z, teleLocs[10].o };
                        break;
#if BOOST_EXPANSION > 0
                    case RACE_DRAENEI:
                        startLocation = { teleLocs[7].map, teleLocs[7].x, teleLocs[7].y, teleLocs[7].z, teleLocs[7].o };
                        break;
                    case RACE_BLOODELF:
                        startLocation = { teleLocs[11].map, teleLocs[11].x, teleLocs[11].y, teleLocs[11].z, teleLocs[11].o };
                        break;
#endif
                    }

                    if (GetConfig()->enableTeleports && (player->GetMapId() != startLocation.map || player->GetDistance2d(startLocation.x, startLocation.y) > 100.0f))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Starting Area", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);

                    if (GetConfig()->enableTaxi && !TaxiNodesKnown(*player))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Learn Azeroth flight paths", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                }

                //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Train available spells", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
                if (GetConfig()->enableSpells)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Train available spells", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);

                if (GetConfig()->enableGearBlue58 && player->GetLevel() >= 58 && !HasStarterSet(player, FullGearListInstant58))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Add Blue Gear. |cFF0008E8 Level 58|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                if (GetConfig()->enableBoost60Bis && player->GetLevel() >= 60 && !HasStarterSet(player, FullGearListBiS60))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Add BiS Gear. |cFF0008E8 Level 60|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
#if BOOST_EXPANSION > 0
                if (GetConfig()->enableBoost70Bis && player->GetLevel() == 70)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Add BiS Gear. |cFF0008E8 Level 70|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
#endif
                if (HasStarterSet(player, FullGearListInstant58))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|cFF8B0000Remove all level 58 blue starter gear.|r", GOSSIP_SENDER_MAIN, 92);
                if (HasStarterSet(player, FullGearListBiS60))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|cFF8B0000Remove all level 60 BiS gear.|r", GOSSIP_SENDER_MAIN, 93);
#if BOOST_EXPANSION > 0
                if (HasStarterSet(player, FullGearListBiS70))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|cFF8B0000Remove all level 70 BiS gear.|r", GOSSIP_SENDER_MAIN, 94);
#endif

                if (GetConfig()->enableMounts && player->GetLevel() >= MOUNT_LEVEL)
                {
                    uint32 groundEpicMount = GetStarterEpicMountForRace(player);
                    uint32 groundBlueMount = GetStarterMountForRace(player);
                    // Flying Mount - Swift Blue Gryphon / Swift Red Wind Rider
                    uint32 flyingMount = player->GetTeam() == ALLIANCE ? 25473 : 25477;
                    if ((player->GetLevel() < 60 && !player->HasItemCount(groundBlueMount, 1)) ||
                        (player->GetLevel() >= 60 && !player->HasItemCount(groundEpicMount, 1)) ||
                        (player->GetLevel() >= 70 && !player->HasItemCount(flyingMount, 1)))
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Give Mount(s)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);

                    }
                }

                if (GetConfig()->enableBags && player->GetLevel() >= 58 && !player->HasItemCount(4500, 4))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Give Bags", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);

                if (GetConfig()->enableFoodRegs && player->GetLevel() >= 58)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Give Food & Reagents", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);

                if (GetConfig()->enableAmmo && player->GetLevel() >= 58 && (player->getClass() == CLASS_HUNTER || player->getClass() == CLASS_ROGUE || player->getClass() == CLASS_WARRIOR))
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
                    if (pItem)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Give Ammo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 26);
                }

                //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Basic Supplies", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);

                if (player->GetLevel() >= 10)
                {
                    if (GetConfig()->enableResetTalents)
                        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Unlearn Talents", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14, "Are you sure?", 0, false);

                    if (player->getClass() == CLASS_HUNTER)
                    {
                        if (GetConfig()->enablePet && !player->GetPet())
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Pet Menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                        else if (GetConfig()->enableUntrainPet && player->GetPet() && !player->GetPet()->m_spells.empty())
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Untrain Pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
                    }
                }

                if (GetConfig()->starterGold && player->GetLevel() >= 58 && player->GetMoney() < GetConfig()->starterGold)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Give 100 gold!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);

                if (GetConfig()->enableFirstAid && player->GetLevel() >= 58)
                {
#if BOOST_EXPANSION == 0
                    if (!player->HasSpell(10846) || !player->HasSpell(SPELL_HEAVY_RUNECLOTH_BAND))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Train |cFF006400First Aid|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
#endif
#if BOOST_EXPANSION == 1
                    if (!player->HasSpell(10846) || (player->GetLevel() >= 60 && !player->HasSpell(SPELL_NETHER_BAND)) || (player->GetLevel() >= 70 && !player->HasSpell(SPELL_HEAVY_NETHER_BAND)))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Train |cFF006400First Aid|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
#endif
                }

#if BOOST_EXPANSION > 0
                if ((!player->GetBoundInstances(DUNGEON_DIFFICULTY_NORMAL).empty() || !player->GetBoundInstances(DUNGEON_DIFFICULTY_HEROIC).empty()) && GetConfig()->enableResetInstances)
#else
                if (!player->GetBoundInstances().empty() && GetConfig()->enableResetInstances)
#endif
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Reset my dungeon/raid lockouts.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16, "Are you sure?", 0, false);

                player->SEND_GOSSIP_MENU(GOSSIP_BOOST_GREET, creature->GetObjectGuid());
                return true;
            }
        }

        return false;
    }

    bool BoostMgr::OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId)
    {
        if (GetConfig()->enabled)
        {
            if (player && creature)
            {
                // Check if speaking with boost npc
                if (creature->GetEntry() != NPC_BOOSTER)
                    return false;

#ifdef ENABLE_PLAYERBOTS
                if (sRandomPlayerbotMgr.IsFreeBot(player))
                    return false;
#endif

                player->GetPlayerMenu()->ClearMenus();

                switch (action)
                {
                // Main menu
                case 29:
                {
                    OnPreGossipHello(player, creature);
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 2:
                {
                    // Remove any gear the character still has from initial creation (now useless)
                    for (uint32 itemEntry : Level1StartingGear)
                        player->DestroyItemCount(itemEntry, 200, true, false);

                    AddStarterSet(player, creature, SET_ID_INSTANT_58);
                    AddAmmo(player);
                    player->GetPlayerMenu()->CloseGossip();
                    break;
                }
                //case GOSSIP_ACTION_INFO_DEF + 20:
                //{
                //    player->CLOSE_GOSSIP_MENU();
                //    BoostPlayer(player, 58);
                //    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                //    // Teleport Player to Dark Portal
                //    if (player->GetTeam() == ALLIANCE)
                //        player->TeleportTo(teleLocs[0].map, teleLocs[0].x, teleLocs[0].y, teleLocs[0].z, teleLocs[0].o);
                //    else
                //        player->TeleportTo(teleLocs[1].map, teleLocs[1].x, teleLocs[1].y, teleLocs[1].z, teleLocs[1].o);
                //    break;
                //}
                case GOSSIP_ACTION_INFO_DEF + 3:
                {
                    player->CLOSE_GOSSIP_MENU();
                    if (player->GetLevel() < 58)
                    {
                        player->GiveLevel(58);
                        player->SetUInt32Value(PLAYER_XP, 0);
                        LearnWeaponSkills(player);
                        LearnArmorSkills(player);
                        player->UpdateSkillsForLevel(true);

                        if (player->getClass() == CLASS_HUNTER)
                        {
                            if (Pet* pet = player->GetPet())
                            {
                                if (pet->GetLevel() < 58)
                                {
                                    pet->GivePetLevel(58);
                                    pet->SavePetToDB(PET_SAVE_AS_CURRENT, player);
                                }
                            }
                        }
                    }
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 30:
                {
                    player->CLOSE_GOSSIP_MENU();
                    BoostPlayer(player, 58);
                    // Add starter gear
                    AddStarterSet(player, creature, SET_ID_INSTANT_58);
                    AddAmmo(player);
                    AddFood(player, 60);
                    AddReagents(player);
                    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
#if BOOST_EXPANSION > 0
                    // Teleport Player to Dark Portal
                    if (player->GetTeam() == ALLIANCE)
                        player->TeleportTo(teleLocs[0].map, teleLocs[0].x, teleLocs[0].y, teleLocs[0].z, teleLocs[0].o);
                    else
                        player->TeleportTo(teleLocs[1].map, teleLocs[1].x, teleLocs[1].y, teleLocs[1].z, teleLocs[1].o);
#else
                    // Teleport Player to Stormwind or Orgrimmar
                    if (player->GetTeam() == ALLIANCE)
                    {
                        GameTele const* tele = sObjectMgr.GetGameTele("stormwind");
                        if (tele)
                        {
                            player->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
                        }
                    }
                    else
                    {
                        GameTele const* tele = sObjectMgr.GetGameTele("orgrimmar");
                        if (tele)
                        {
                            player->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
                        }
                    }
#endif
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 4:
                {
                    player->CLOSE_GOSSIP_MENU();
                    if (player->GetLevel() < 60)
                    {
                        player->GiveLevel(60);
                        player->SetUInt32Value(PLAYER_XP, 0);
                        LearnWeaponSkills(player);
                        LearnArmorSkills(player);
                        player->UpdateSkillsForLevel(true);

                        if (player->getClass() == CLASS_HUNTER)
                        {
                            if (Pet* pet = player->GetPet())
                            {
                                if (pet->GetLevel() < 60)
                                {
                                    pet->GivePetLevel(60);
                                    pet->SavePetToDB(PET_SAVE_AS_CURRENT, player);
                                }
                            }
                        }
                    }
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 40:
                {
                    player->CLOSE_GOSSIP_MENU();
                    BoostPlayer(player, 60);
                    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
#if BOOST_EXPANSION > 0
                    // Teleport Player to Dark Portal
                    if (player->GetTeam() == ALLIANCE)
                        player->TeleportTo(teleLocs[0].map, teleLocs[0].x, teleLocs[0].y, teleLocs[0].z, teleLocs[0].o);
                    else
                        player->TeleportTo(teleLocs[1].map, teleLocs[1].x, teleLocs[1].y, teleLocs[1].z, teleLocs[1].o);
#else
                    // Teleport Player to Stormwind or Orgrimmar
                    if (player->GetTeam() == ALLIANCE)
                    {
                        GameTele const* tele = sObjectMgr.GetGameTele("stormwind");
                        if (tele)
                        {
                            player->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
                        }
                    }
                    else
                    {
                        GameTele const* tele = sObjectMgr.GetGameTele("orgrimmar");
                        if (tele)
                        {
                            player->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
                        }
                    }
#endif
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 5:
                {
                    player->CLOSE_GOSSIP_MENU();
                    if (player->GetLevel() < 70)
                    {
                        player->GiveLevel(70);
                        player->SetUInt32Value(PLAYER_XP, 0);
                        LearnWeaponSkills(player);
                        LearnArmorSkills(player);
                        player->UpdateSkillsForLevel(true);

                        if (player->getClass() == CLASS_HUNTER)
                        {
                            if (Pet* pet = player->GetPet())
                            {
                                if (pet->GetLevel() < 70)
                                {
                                    pet->GivePetLevel(70);
                                    pet->SavePetToDB(PET_SAVE_AS_CURRENT, player);
                                }
                            }
                        }
                    }
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 50:
                {
                    player->CLOSE_GOSSIP_MENU();
                    BoostPlayer(player, 70);
                    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                    // Teleport Player To Shattrath City
                    if (player->GetTeam() == ALLIANCE)
                    {
                        player->TeleportTo(teleLocs[2].map, teleLocs[2].x, teleLocs[2].y, teleLocs[2].z, teleLocs[2].o);
                        player->SetHomebindToLocation(WorldLocation(teleLocs[2].map, teleLocs[2].x, teleLocs[2].y, teleLocs[2].z, teleLocs[2].o), teleLocs[2].area);
                    }
                    else
                    {
                        player->TeleportTo(teleLocs[3].map, teleLocs[3].x, teleLocs[3].y, teleLocs[3].z, teleLocs[3].o);
                        player->SetHomebindToLocation(WorldLocation(teleLocs[3].map, teleLocs[3].x, teleLocs[3].y, teleLocs[3].z, teleLocs[3].o), teleLocs[3].area);
                    }
                    break;
                }
                // Teleport Only - Shattrath City or Capital City
                case GOSSIP_ACTION_INFO_DEF + 6:
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
#if BOOST_EXPANSION > 0
                    // Teleport Player To Shattrath City
                    if (player->GetTeam() == ALLIANCE)
                        player->TeleportTo(teleLocs[2].map, teleLocs[2].x, teleLocs[2].y, teleLocs[2].z, teleLocs[2].o);
                    else
                        player->TeleportTo(teleLocs[3].map, teleLocs[3].x, teleLocs[3].y, teleLocs[3].z, teleLocs[3].o);
#else
                    // Teleport Player to Stormwind or Orgrimmar
                    if (player->GetTeam() == ALLIANCE)
                    {
                        GameTele const* tele = sObjectMgr.GetGameTele("stormwind");
                        if (tele)
                        {
                            player->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
                        }
                    }
                    else
                    {
                        GameTele const* tele = sObjectMgr.GetGameTele("orgrimmar");
                        if (tele)
                        {
                            player->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
                        }
                    }
#endif
                    break;
                }
                // Teleport Only - Dark Portal
                case GOSSIP_ACTION_INFO_DEF + 7:
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                    // Teleport Player to Dark Portal
                    if (player->GetTeam() == ALLIANCE)
                        player->TeleportTo(teleLocs[0].map, teleLocs[0].x, teleLocs[0].y, teleLocs[0].z, teleLocs[0].o);
                    else
                        player->TeleportTo(teleLocs[1].map, teleLocs[1].x, teleLocs[1].y, teleLocs[1].z, teleLocs[1].o);
                    break;
                }
                // Teleport Only - Starting Area
                case GOSSIP_ACTION_INFO_DEF + 24:
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                    switch (player->getRace())
                    {
                    case RACE_HUMAN:
                        player->TeleportTo(teleLocs[4].map, teleLocs[4].x, teleLocs[4].y, teleLocs[4].z, teleLocs[4].o);
                        break;
                    case RACE_DWARF:
                    case RACE_GNOME:
                        player->TeleportTo(teleLocs[5].map, teleLocs[5].x, teleLocs[5].y, teleLocs[5].z, teleLocs[5].o);
                        break;
                    case RACE_NIGHTELF:
                        player->TeleportTo(teleLocs[6].map, teleLocs[6].x, teleLocs[6].y, teleLocs[6].z, teleLocs[6].o);
                        break;
                    case RACE_TROLL:
                    case RACE_ORC:
                        player->TeleportTo(teleLocs[8].map, teleLocs[8].x, teleLocs[8].y, teleLocs[8].z, teleLocs[8].o);
                        break;
                    case RACE_UNDEAD:
                        player->TeleportTo(teleLocs[9].map, teleLocs[9].x, teleLocs[9].y, teleLocs[9].z, teleLocs[9].o);
                        break;
                    case RACE_TAUREN:
                        player->TeleportTo(teleLocs[10].map, teleLocs[10].x, teleLocs[10].y, teleLocs[10].z, teleLocs[10].o);
                        break;
#if BOOST_EXPANSION > 0
                    case RACE_DRAENEI:
                        player->TeleportTo(teleLocs[7].map, teleLocs[7].x, teleLocs[7].y, teleLocs[7].z, teleLocs[7].o);
                        break;
                    case RACE_BLOODELF:
                        player->TeleportTo(teleLocs[11].map, teleLocs[11].x, teleLocs[11].y, teleLocs[11].z, teleLocs[11].o);
                        break;
#endif
                    }
                    break;
                }
                // Classic Full Best in Slot
                // Pure DPS classes Hunter, Rogue, Mage, and Warlock don't require a secondary selection
                case GOSSIP_ACTION_INFO_DEF + 8:
                {
                    switch (player->getClass())
                    {
                    case CLASS_ROGUE:
                        GivePlayerItems(player, creature, Lvl60BiS_Rogue);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_MAGE:
                        GivePlayerItems(player, creature, Lvl60BiS_Mage);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_HUNTER:
                        GivePlayerItems(player, creature, Lvl60BiS_Hunter);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_WARLOCK:
                        GivePlayerItems(player, creature, Lvl60BiS_Warlock);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_SHAMAN:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Restoration", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2001);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Enhancement", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2002);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elemental", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2003);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_PRIEST:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shadow", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2004);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Discipline/Holy", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2005);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_PALADIN:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Holy", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2006);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Retribution", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2007);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Protection", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2008);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_WARRIOR:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Fury/Arms", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2009);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Protection", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2010);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_DRUID:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Feral (Cat/DPS)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2011);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Feral (Bear/Tank)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2012);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Balance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2013);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Restoration", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2014);
                        player->SendPreparedGossip(creature);
                        break;
                    }
                    break;
                }
                // Classic Full Best in Slot (Spec Selected)
                    // Shaman - Restoration
                case GOSSIP_ACTION_INFO_DEF + 2001: GivePlayerItems(player, creature, Lvl60BiS_ShamanResto); player->GetPlayerMenu()->CloseGossip(); break;
                    // Shaman - Enhancement
                case GOSSIP_ACTION_INFO_DEF + 2002: GivePlayerItems(player, creature, Lvl60BiS_ShamanEnhancement); player->GetPlayerMenu()->CloseGossip(); break;
                    // Shaman - Elemental
                case GOSSIP_ACTION_INFO_DEF + 2003: GivePlayerItems(player, creature, Lvl60BiS_ShamanElemental); player->GetPlayerMenu()->CloseGossip(); break;
                    // Priest - Shadow
                case GOSSIP_ACTION_INFO_DEF + 2004: GivePlayerItems(player, creature, Lvl60BiS_PriestShadow); player->GetPlayerMenu()->CloseGossip(); break;
                    // Priest - Discipline/Holy
                case GOSSIP_ACTION_INFO_DEF + 2005: GivePlayerItems(player, creature, Lvl60BiS_PriestDiscHoly); player->GetPlayerMenu()->CloseGossip(); break;
                    // Paladin - Holy
                case GOSSIP_ACTION_INFO_DEF + 2006: GivePlayerItems(player, creature, Lvl60BiS_PaladinHoly); player->GetPlayerMenu()->CloseGossip(); break;
                    // Paladin - Retribution
                case GOSSIP_ACTION_INFO_DEF + 2007: GivePlayerItems(player, creature, Lvl60BiS_PaladinRetribution); player->GetPlayerMenu()->CloseGossip(); break;
                    // Paladin - Protection
                case GOSSIP_ACTION_INFO_DEF + 2008: GivePlayerItems(player, creature, Lvl60BiS_PaladinProtection); player->GetPlayerMenu()->CloseGossip(); break;
                    // Warrior - Fury/Arms
                case GOSSIP_ACTION_INFO_DEF + 2009: GivePlayerItems(player, creature, Lvl60BiS_WarriorFuryArms); player->GetPlayerMenu()->CloseGossip(); break;
                    // Warrior - Protection
                case GOSSIP_ACTION_INFO_DEF + 2010: GivePlayerItems(player, creature, Lvl60BiS_WarriorProtection); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Feral (Cat/DPS)
                case GOSSIP_ACTION_INFO_DEF + 2011: GivePlayerItems(player, creature, Lvl60BiS_DruidFeralCat); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Feral (Bear/Tank)
                case GOSSIP_ACTION_INFO_DEF + 2012: GivePlayerItems(player, creature, Lvl60BiS_DruidFeralBear); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Balance
                case GOSSIP_ACTION_INFO_DEF + 2013: GivePlayerItems(player, creature, Lvl60BiS_DruidBalance); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Restoration
                case GOSSIP_ACTION_INFO_DEF + 2014: GivePlayerItems(player, creature, Lvl60BiS_DruidResto); player->GetPlayerMenu()->CloseGossip(); break;
                // TBC Full Best in Slot
                case GOSSIP_ACTION_INFO_DEF + 9:
                {
                    switch (player->getClass())
                    {
                    case CLASS_ROGUE:
                        GivePlayerItems(player, creature, Lvl70BiS_Rogue);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_MAGE:
                        GivePlayerItems(player, creature, Lvl70BiS_Mage);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_HUNTER:
                        GivePlayerItems(player, creature, Lvl70BiS_Hunter);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_WARLOCK:
                        GivePlayerItems(player, creature, Lvl70BiS_Warlock);
                        player->CLOSE_GOSSIP_MENU();
                        break;
                    case CLASS_SHAMAN:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Restoration", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3001);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Enhancement", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3002);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elemental", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3003);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_PRIEST:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shadow", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3004);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Discipline/Holy", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3005);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_PALADIN:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Holy", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3006);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Retribution", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3007);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Protection", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3008);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_WARRIOR:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Fury/Arms", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3009);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Protection", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3010);
                        player->SendPreparedGossip(creature);
                        break;
                    case CLASS_DRUID:
                        player->PrepareGossipMenu(creature, GOSSIP_BOOST_PICK_SPEC);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Feral (Cat/DPS)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3011);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Feral (Bear/Tank)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3012);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Balance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3013);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Restoration", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3014);
                        player->SendPreparedGossip(creature);
                        break;
                    }
                    break;
                }
                // TBC Full Best in Slot (Spec Selected)
                // Shaman - Restoration
                case GOSSIP_ACTION_INFO_DEF + 3001: GivePlayerItems(player, creature, Lvl70BiS_ShamanResto); player->GetPlayerMenu()->CloseGossip(); break;
                    // Shaman - Enhancement
                case GOSSIP_ACTION_INFO_DEF + 3002: GivePlayerItems(player, creature, Lvl70BiS_ShamanEnhancement); player->GetPlayerMenu()->CloseGossip(); break;
                    // Shaman - Elemental
                case GOSSIP_ACTION_INFO_DEF + 3003: GivePlayerItems(player, creature, Lvl70BiS_ShamanElemental); player->GetPlayerMenu()->CloseGossip(); break;
                    // Priest - Shadow
                case GOSSIP_ACTION_INFO_DEF + 3004: GivePlayerItems(player, creature, Lvl70BiS_PriestShadow); player->GetPlayerMenu()->CloseGossip(); break;
                    // Priest - Discipline/Holy
                case GOSSIP_ACTION_INFO_DEF + 3005: GivePlayerItems(player, creature, Lvl70BiS_PriestDiscHoly); player->GetPlayerMenu()->CloseGossip(); break;
                    // Paladin - Holy
                case GOSSIP_ACTION_INFO_DEF + 3006: GivePlayerItems(player, creature, Lvl70BiS_PaladinHoly); player->GetPlayerMenu()->CloseGossip(); break;
                    // Paladin - Retribution
                case GOSSIP_ACTION_INFO_DEF + 3007: GivePlayerItems(player, creature, Lvl70BiS_PaladinRetribution); player->GetPlayerMenu()->CloseGossip(); break;
                    // Paladin - Protection
                case GOSSIP_ACTION_INFO_DEF + 3008: GivePlayerItems(player, creature, Lvl70BiS_PaladinProtection); player->GetPlayerMenu()->CloseGossip(); break;
                    // Warrior - Fury/Arms
                case GOSSIP_ACTION_INFO_DEF + 3009: GivePlayerItems(player, creature, Lvl70BiS_WarriorFuryArms); player->GetPlayerMenu()->CloseGossip(); break;
                    // Warrior - Protection
                case GOSSIP_ACTION_INFO_DEF + 3010: GivePlayerItems(player, creature, Lvl70BiS_WarriorProtection); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Feral (Cat/DPS)
                case GOSSIP_ACTION_INFO_DEF + 3011: GivePlayerItems(player, creature, Lvl70BiS_DruidFeralCat); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Feral (Bear/Tank)
                case GOSSIP_ACTION_INFO_DEF + 3012: GivePlayerItems(player, creature, Lvl70BiS_DruidFeralBear); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Balance
                case GOSSIP_ACTION_INFO_DEF + 3013: GivePlayerItems(player, creature, Lvl70BiS_DruidBalance); player->GetPlayerMenu()->CloseGossip(); break;
                    // Druid - Restoration
                case GOSSIP_ACTION_INFO_DEF + 3014: GivePlayerItems(player, creature, Lvl70BiS_DruidResto); player->GetPlayerMenu()->CloseGossip(); break;
                // Add a mount
                case GOSSIP_ACTION_INFO_DEF + 12:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    if (player->GetLevel() < MOUNT_LEVEL)
                        break;

                    uint32 groundEpicMount = GetStarterEpicMountForRace(player);
                    uint32 groundBlueMount = GetStarterMountForRace(player);
                    // Flying Mount - Swift Blue Gryphon / Swift Red Wind Rider
                    uint32 flyingMount = player->GetTeam() == ALLIANCE ? 25473 : 25477;

                    if (player->GetLevel() < 60 && !player->HasItemCount(groundBlueMount, 1))
                    {
                        player->StoreNewItemInBestSlots(groundBlueMount, 1);
                    }
                    if (player->GetLevel() >= 60 && !player->HasItemCount(groundEpicMount, 1))
                    {
                        player->StoreNewItemInBestSlots(groundEpicMount, 1);
                    }
                    if (player->GetLevel() >= 70 && !player->HasItemCount(flyingMount, 1))
                    {
                        player->StoreNewItemInBestSlots(flyingMount, 1);
                    }

                    if (player->GetLevel() < 60)
                        player->CastSpell(player, SPELL_APPRENTICE_RIDING, TRIGGERED_OLD_TRIGGERED);
                    else if (player->GetLevel() < 70)
                        player->CastSpell(player, SPELL_JOURNEYMAN_RIDING, TRIGGERED_OLD_TRIGGERED);
                    else if (player->GetLevel() == 70)
                        player->CastSpell(player, SPELL_ARTISAN_RIDING, TRIGGERED_OLD_TRIGGERED);

                    break;
                }
                // Learn Azeroth Taxi paths
                case GOSSIP_ACTION_INFO_DEF + 13:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    TaxiNodesTeach(*player);
                    //player->SetFacingToObject(creature);
                    player->HandleEmote(EMOTE_ONESHOT_SALUTE);
                    break;
                }
                // Unlearn Talents
                case GOSSIP_ACTION_INFO_DEF + 14:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    player->resetTalents(true);
                    ChatHandler(player).SendSysMessage(LANG_RESET_TALENTS);
                    break;
                }
                // Money!
                case GOSSIP_ACTION_INFO_DEF + 15:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    player->ModifyMoney((int32)GetConfig()->starterGold);
                    break;
                }
                // Unbind Instances
                case GOSSIP_ACTION_INFO_DEF + 16:
                {
                    player->GetPlayerMenu()->CloseGossip();
#if BOOST_EXPANSION > 0
                    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
                    {
                        Player::BoundInstancesMap& binds = player->GetBoundInstances(Difficulty(i));
                        for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
                        {
                            if (itr->first != player->GetMapId())
                            {
                                DungeonPersistentState* save = itr->second.state;
                                player->UnbindInstance(itr, Difficulty(i));
                            }
                            else
                                ++itr;
                        }
                    }
#else
                    Player::BoundInstancesMap& binds = player->GetBoundInstances();
                    for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
                    {
                        if (itr->first != player->GetMapId())
                        {
                            DungeonPersistentState* save = itr->second.state;
                            player->UnbindInstance(itr);
                        }
                        else
                            ++itr;
                    }
#endif
                    break;
                }
                // Pets - Page 1
                case GOSSIP_ACTION_INFO_DEF + 17:
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back to Main Menu", GOSSIP_SENDER_MAIN, 29);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Next Page ->", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bat", GOSSIP_SENDER_MAIN, 501);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bear", GOSSIP_SENDER_MAIN, 502);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Boar", GOSSIP_SENDER_MAIN, 503);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Carrion Bird", GOSSIP_SENDER_MAIN, 504);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Cat", GOSSIP_SENDER_MAIN, 505);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Crab", GOSSIP_SENDER_MAIN, 506);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Crocolisk", GOSSIP_SENDER_MAIN, 507);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Gorilla", GOSSIP_SENDER_MAIN, 508);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Hyena", GOSSIP_SENDER_MAIN, 509);
                    player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PETS, creature->GetObjectGuid());
                    break;
                }
                // Pets - Page 2
                case GOSSIP_ACTION_INFO_DEF + 18:
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back to Main Menu", GOSSIP_SENDER_MAIN, 29);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Previous Page", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Owl", GOSSIP_SENDER_MAIN, 510);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Raptor", GOSSIP_SENDER_MAIN, 511);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strider", GOSSIP_SENDER_MAIN, 512);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Scorpid", GOSSIP_SENDER_MAIN, 513);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Serpent", GOSSIP_SENDER_MAIN, 514);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Spider", GOSSIP_SENDER_MAIN, 515);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Turtle", GOSSIP_SENDER_MAIN, 516);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Wolf", GOSSIP_SENDER_MAIN, 517);
                    player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PETS, creature->GetObjectGuid());
                    break;
                }
                // Untrain Pet
                case GOSSIP_ACTION_INFO_DEF + 19:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    Pet* pPet = player->GetPet();

                    if (pPet && pPet->m_spells.size() >= 1)
                    {
                        CharmInfo* charmInfo = pPet->GetCharmInfo();
                        if (charmInfo)
                        {
                            for (PetSpellMap::iterator itr = pPet->m_spells.begin(); itr != pPet->m_spells.end();)
                            {
                                uint32 spell_id = itr->first;
                                ++itr;
                                pPet->unlearnSpell(spell_id, false);
                            }

                            uint32 cost = pPet->resetTalentsCost();

                            pPet->SetTP(pPet->GetLevel() * (pPet->GetLoyaltyLevel() - 1));

                            for (int i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
                                if (UnitActionBarEntry const* ab = charmInfo->GetActionBarEntry(i))
                                    if (ab->GetAction() && ab->IsActionBarForSpell())
                                        charmInfo->SetActionBar(i, 0, ACT_DISABLED);

                            // relearn pet passives
                            pPet->LearnPetPassives();

                            pPet->m_resetTalentsTime = time(nullptr);
                            pPet->m_resetTalentsCost = cost;

                            player->PetSpellInitialize();
                        }
                    }
                    break;
                }
                // Learn available spells
                case GOSSIP_ACTION_INFO_DEF + 20:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    LearnAvailableClassSpells(player, false);
                    break;
                }
                // Learn available high lvl quest rewarded spells
                case GOSSIP_ACTION_INFO_DEF + 21:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    LearnAvailableClassSpells(player, true);
                    break;
                }
                // Learn First Aid
                case GOSSIP_ACTION_INFO_DEF + 22:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    player->CastSpell(player, SPELL_ARTISAN_FIRST_AID, TRIGGERED_OLD_TRIGGERED);
                    player->learnSpell(SPELL_HEAVY_RUNECLOTH_BAND, false);
                    if (player->GetLevel() >= 60)
                    {
                        player->CastSpell(player, SPELL_MASTER_FIRST_AID, TRIGGERED_OLD_TRIGGERED);
                        player->learnSpell(SPELL_NETHER_BAND, false);
                        if (player->GetLevel() >= 70)
                            player->learnSpell(SPELL_HEAVY_NETHER_BAND, false);
                    }

                    SkillLineEntry const* sl = sSkillLineStore.LookupEntry(SKILL_FIRST_AID);
                    if (sl)
                    {
                        uint32 maxSkill = player->GetSkillMaxPure(SKILL_FIRST_AID);

                        if (player->GetSkillValue(SKILL_FIRST_AID) < maxSkill)
                            player->SetSkill(SKILL_FIRST_AID, maxSkill, maxSkill);
                    }
                    break;
                }
                // Give Bags
                case GOSSIP_ACTION_INFO_DEF + 23:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    // Traveller's Backpack
                    if (!player->HasItemCount(4500, 4))
                        player->StoreNewItemInBestSlots(4500, 4);
                    break;
                }
                // Give Food & Reagents
                case GOSSIP_ACTION_INFO_DEF + 25:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    AddFood(player, 60);
                    AddReagents(player);
                    break;
                }
                // Give Ammo
                case GOSSIP_ACTION_INFO_DEF + 26:
                {
                    player->GetPlayerMenu()->CloseGossip();
                    AddAmmo(player);
                    break;
                }
                default:
                {
                    // Main Menu
                    if (action == 29)
                        OnPreGossipHello(player, creature);
                    else if (action == GOSSIP_OPTION_VENDOR)
                        player->GetSession()->SendListInventory(creature->GetObjectGuid());
                    else if (action == GOSSIP_OPTION_TRAINER)
                        player->GetSession()->SendTrainerList(creature->GetObjectGuid());
                    // Remove Starter Set - Instant 58
                    else if (action == 92)
                    {
                        RemoveStarterSet(player, FullGearListInstant58);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    // Remove Starter Set - BiS 60
                    else if (action == 93)
                    {
                        RemoveStarterSet(player, FullGearListBiS60);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    // Remove Starter Set - BiS 70
                    else if (action == 94)
                    {
                        RemoveStarterSet(player, FullGearListBiS70);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    // Remove Starter Set - Custom 70 Uncommon
                    else if (action == 95)
                    {
                        RemoveStarterSet(player, FullGearList);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    // Choose Instant 70 Starter Set
                    else if (action == 99)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back to Main Menu", GOSSIP_SENDER_MAIN, 29);
                        switch (player->getClass())
                        {
                        case CLASS_WARRIOR:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Arms", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Fury", GOSSIP_SENDER_MAIN, 101);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Protection", GOSSIP_SENDER_MAIN, 102);
                            break;
                        case CLASS_PALADIN:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Holy", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Protection", GOSSIP_SENDER_MAIN, 101);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Retribution", GOSSIP_SENDER_MAIN, 102);
                            break;
                        case CLASS_PRIEST:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Discipline", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Holy", GOSSIP_SENDER_MAIN, 101);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shadow", GOSSIP_SENDER_MAIN, 102);
                            break;
                        case CLASS_DRUID:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Balance", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Feral", GOSSIP_SENDER_MAIN, 101);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Restoration", GOSSIP_SENDER_MAIN, 102);
                            break;
                        case CLASS_SHAMAN:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elemental", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Enhancement", GOSSIP_SENDER_MAIN, 101);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Restoration", GOSSIP_SENDER_MAIN, 102);
                            break;
                        case CLASS_ROGUE:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Assassination", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Combat", GOSSIP_SENDER_MAIN, 101);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Subtlety", GOSSIP_SENDER_MAIN, 102);
                            break;

                            // these only give the illusion of choice
                        case CLASS_WARLOCK:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Affliction", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Demonology", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Destruction", GOSSIP_SENDER_MAIN, 100);
                            break;
                        case CLASS_HUNTER:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Beast Mastery", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Marksmanship", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Survival", GOSSIP_SENDER_MAIN, 100);
                            break;
                        case CLASS_MAGE:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Arcane", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Fire", GOSSIP_SENDER_MAIN, 100);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Frost", GOSSIP_SENDER_MAIN, 100);
                            break;
                        }
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_SPEC, creature->GetObjectGuid());
                    }
                    // Primary - Specialization 1
                    else if (action == 100)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back to Spec Selection", GOSSIP_SENDER_MAIN, 99);
                        OfferPrimarySecondaryModChoices(player, 200, 0);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_1, creature->GetObjectGuid());
                    }
                    // Secondary - Specialization 1
                    else if (action > 200 && action < 300)
                    {
                        AddStarterSet(player, creature, SET_ID_PRIMARY, action - 200);
                        OfferPrimarySecondaryModChoices(player, 300, 0);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_2, creature->GetObjectGuid());
                    }
                    // Tertiary - Specialization 1
                    else if (action > 300 && action < 400)
                    {
                        AddStarterSet(player, creature, SET_ID_SECONDARY, action - 300);
                        OfferTertiaryModChoices(player, 600, 0);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_3, creature->GetObjectGuid());
                    }
                    // Tertiary - Specialization 1 (cont.)
                    else if (action > 600 && action < 700)
                    {
                        AddStarterSet(player, creature, SET_ID_TERTIARY, action - 600, 0);
                        player->GetPlayerMenu()->CloseGossip();
                    }
                    // Primary - Specialization 2
                    else if (action == 101)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back to Spec Selection", GOSSIP_SENDER_MAIN, 99);
                        OfferPrimarySecondaryModChoices(player, 700, 1);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_1, creature->GetObjectGuid());
                    }
                    // Secondary - Specialization 2
                    else if (action > 700 && action < 800)
                    {
                        AddStarterSet(player, creature, SET_ID_PRIMARY, action - 700);
                        OfferPrimarySecondaryModChoices(player, 800, 1);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_2, creature->GetObjectGuid());
                    }
                    // Tertiary - Specialization 2
                    else if (action > 800 && action < 900)
                    {
                        AddStarterSet(player, creature, SET_ID_SECONDARY, action - 800);
                        OfferTertiaryModChoices(player, 900, 1);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_3, creature->GetObjectGuid());
                    }
                    // Tertiary - Specialization 2 (cont.)
                    else if (action > 900 && action < 1000)
                    {
                        AddStarterSet(player, creature, SET_ID_TERTIARY, action - 900, 1);
                        player->GetPlayerMenu()->CloseGossip();
                    }

                    // Primary - Specialization 3
                    else if (action == 102)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back to Spec Selection", GOSSIP_SENDER_MAIN, 99);
                        OfferPrimarySecondaryModChoices(player, 1000, 2);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_1, creature->GetObjectGuid());
                    }
                    // Secondary - Specialization 3
                    else if (action > 1000 && action < 1100)
                    {
                        AddStarterSet(player, creature, SET_ID_PRIMARY, action - 1000);
                        OfferPrimarySecondaryModChoices(player, 1100, 2);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_2, creature->GetObjectGuid());
                    }
                    // Tertiary - Specialization 3
                    else if (action > 1100 && action < 1200)
                    {
                        AddStarterSet(player, creature, SET_ID_SECONDARY, action - 1100);
                        OfferTertiaryModChoices(player, 1200, 2);
                        player->SEND_GOSSIP_MENU(GOSSIP_BOOST_PICK_GEAR_3, creature->GetObjectGuid());
                    }
                    // Tertiary - Specialization 3 (cont.)
                    else if (action > 1200 && action < 1300)
                    {
                        AddStarterSet(player, creature, SET_ID_TERTIARY, action - 1200, 2);
                        player->GetPlayerMenu()->CloseGossip();
                    }
                    // Pets
                    else if (action == 501) CreatePet(player, creature, NPC_PET_BAT);
                    else if (action == 502) CreatePet(player, creature, NPC_PET_BEAR);
                    else if (action == 503) CreatePet(player, creature, NPC_PET_BOAR);
                    else if (action == 504) CreatePet(player, creature, NPC_PET_CARRION);
                    else if (action == 505) CreatePet(player, creature, NPC_PET_CAT);
                    else if (action == 506) CreatePet(player, creature, NPC_PET_CRAB);
                    else if (action == 507) CreatePet(player, creature, NPC_PET_CROC);
                    else if (action == 508) CreatePet(player, creature, NPC_PET_GORILLA);
                    else if (action == 509) CreatePet(player, creature, NPC_PET_HYENA);
                    else if (action == 510) CreatePet(player, creature, NPC_PET_OWL);
                    else if (action == 511) CreatePet(player, creature, NPC_PET_RAPTOR);
                    else if (action == 512) CreatePet(player, creature, NPC_PET_STRIDER);
                    else if (action == 513) CreatePet(player, creature, NPC_PET_SCORPID);
                    else if (action == 514) CreatePet(player, creature, NPC_PET_SERPENT);
                    else if (action == 515) CreatePet(player, creature, NPC_PET_SPIDER);
                    else if (action == 516) CreatePet(player, creature, NPC_PET_TURTLE);
                    else if (action == 517) CreatePet(player, creature, NPC_PET_WOLF);
                }
                }

                return true;
            }
        }

        return false;
    }

    void BoostMgr::OnAddToWorld(Creature* creature)
    {
        // Check if speaking with boost npc.
        if (creature->GetEntry() != NPC_BOOSTER)
            return;

        if (!GetConfig()->enabled)
        {
            creature->SetRespawnDelay(RESPAWN_ONE_DAY);
            creature->ForcedDespawn();
        }
        else
        {
            creature->Respawn();
        }
    }

    void BoostMgr::BoostPlayer(Player* player, uint32 targetLevel)
    {
        // Learn weapon skills
        LearnWeaponSkills(player);
        // Learn armor skills
        LearnArmorSkills(player);

        if (player->GetLevel() < targetLevel)
        {
            player->GiveLevel(targetLevel);
            player->SetUInt32Value(PLAYER_XP, 0);
            LearnAvailableClassSpells(player, true);
            player->UpdateSkillsForLevel(true);

            if (player->getClass() == CLASS_HUNTER)
            {
                if (Pet* pet = player->GetPet())
                {
                    if (pet->GetLevel() < targetLevel)
                    {
                        pet->GivePetLevel(targetLevel);
                        pet->SavePetToDB(PET_SAVE_AS_CURRENT, player);
                    }
                }
            }
        }

        // Learn skills
        player->CastSpell(player, SPELL_ARTISAN_FIRST_AID, TRIGGERED_OLD_TRIGGERED);
        player->CastSpell(player, SPELL_HEAVY_RUNECLOTH_BAND, TRIGGERED_OLD_TRIGGERED);
        switch (targetLevel)
        {
        case 58:
            player->CastSpell(player, SPELL_APPRENTICE_RIDING, TRIGGERED_OLD_TRIGGERED);
            break;
        case 60:
            player->CastSpell(player, SPELL_JOURNEYMAN_RIDING, TRIGGERED_OLD_TRIGGERED);
            break;
        case 70:
            player->CastSpell(player, SPELL_ARTISAN_RIDING, TRIGGERED_OLD_TRIGGERED);
            player->CastSpell(player, SPELL_MASTER_FIRST_AID, TRIGGERED_OLD_TRIGGERED);
            player->learnSpell(SPELL_HEAVY_NETHER_BAND, false);
            break;
        }

        SetMaxSkill(player, SKILL_FIRST_AID);

        // Remove any gear the character still has from initial creation (now useless)
        for (uint32 itemEntry : Level1StartingGear)
            player->DestroyItemCount(itemEntry, 200, true, false);

        // Traveller's Backpack
        if (!player->HasItemCount(4500, 4))
            player->StoreNewItemInBestSlots(4500, 4);

        if (targetLevel == 58)
        {
            // Ground Mount
            uint32 groundMount = GetStarterMountForRace(player);
            if (!player->HasItemCount(groundMount, 1))
                player->StoreNewItemInBestSlots(groundMount, 1);
        }
        else
        {
            // Epic Ground Mount
            uint32 groundMount = GetStarterEpicMountForRace(player);
            if (!player->HasItemCount(groundMount, 1))
                player->StoreNewItemInBestSlots(groundMount, 1);

            // Flying Mount - Swift Blue Gryphon / Swift Red Wind Rider
            uint32 flyingMount = player->GetTeam() == ALLIANCE ? 25473 : 25477;
            if (targetLevel == 70 && !player->HasItemCount(flyingMount, 1))
                player->StoreNewItemInBestSlots(flyingMount, 1);
        }

        TaxiNodesTeach(*player);

        if (player->GetMoney() < 1000000)
            player->ModifyMoney(1000000);

        player->SaveToDB();
    }

    void BoostMgr::LearnAvailableClassSpells(Player* player, bool includeQuestRewards)
    {
        ChrClassesEntry const* clsEntry = sChrClassesStore.LookupEntry(player->getClass());
        if (!clsEntry)
            return;

        uint32 family = clsEntry->spellfamily;

        // special cases which aren't sourced from trainers and normally require quests to obtain - added here for convenience
        ObjectMgr::QuestMap const& qTemplates = sObjectMgr.GetQuestTemplates();
        for (const auto& qTemplate : qTemplates)
        {
            Quest* quest = qTemplate.second.get();
            if (!quest)
                continue;

            // only class quests player could do
            if (quest->GetRequiredClasses() == 0 || !player->SatisfyQuestClass(quest, false) || !player->SatisfyQuestRace(quest, false) || !player->SatisfyQuestLevel(quest, false))
                continue;

            // custom filter for scripting purposes
            if (!includeQuestRewards && quest->GetMinLevel() >= 60)
                continue;

            player->learnQuestRewardedSpells(quest);
        }

        // learn trainer spells
        for (uint32 id = 0; id < sCreatureStorage.GetMaxEntry(); ++id)
        {
            CreatureInfo const* co = sCreatureStorage.LookupEntry<CreatureInfo>(id);
            if (!co)
                continue;

            if (co->TrainerType != TRAINER_TYPE_CLASS)
                continue;

            if (co->TrainerType == TRAINER_TYPE_CLASS && co->TrainerClass != player->getClass())
                continue;

            uint32 trainerId = co->TrainerTemplateId;
            if (!trainerId)
                trainerId = co->Entry;

            TrainerSpellData const* trainer_spells = sObjectMgr.GetNpcTrainerTemplateSpells(trainerId);
            if (!trainer_spells)
                trainer_spells = sObjectMgr.GetNpcTrainerSpells(trainerId);

            if (!trainer_spells)
                continue;

            for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
            {
                TrainerSpell const* tSpell = &itr->second;

                if (!tSpell)
                    continue;

                uint32 reqLevel = 0;

                // skip wrong class/race skills
                if (!player->IsSpellFitByClassAndRace(tSpell->spell, &reqLevel))
                    continue;

                if (tSpell->conditionId && !sObjectMgr.IsConditionSatisfied(tSpell->conditionId, player, player->GetMap(), player, CONDITION_FROM_TRAINER))
                    continue;

                // skip spells with first rank learned as talent (and all talents then also)
                uint32 first_rank = sSpellMgr.GetFirstSpellInChain(tSpell->spell);
                reqLevel = tSpell->isProvidedReqLevel ? tSpell->reqLevel : std::max(reqLevel, tSpell->reqLevel);
                bool isValidTalent = GetTalentSpellCost(first_rank) && player->HasSpell(first_rank) && reqLevel <= player->GetLevel();

                TrainerSpellState state = player->GetTrainerSpellState(tSpell, reqLevel);
                if (state != TRAINER_SPELL_GREEN && !isValidTalent)
                    continue;

                SpellEntry const* proto = sSpellTemplate.LookupEntry<SpellEntry>(tSpell->spell);
                if (!proto)
                    continue;

                // fix activate state for non-stackable low rank (and find next spell for !active case)
                if (uint32 nextId = sSpellMgr.GetSpellBookSuccessorSpellId(proto->Id))
                {
                    if (player->HasSpell(nextId))
                    {
                        // high rank already known so this must !active
                        continue;
                    }
                }

                // skip other spell families (minus a few exceptions)
                if (proto->SpellFamilyName != family)
                {
                    SkillLineAbilityMapBounds bounds = sSpellMgr.GetSkillLineAbilityMapBoundsBySpellId(tSpell->spell);
                    if (bounds.first == bounds.second)
                        continue;

                    SkillLineAbilityEntry const* skillInfo = bounds.first->second;
                    if (!skillInfo)
                        continue;

                    switch (skillInfo->skillId)
                    {
                    case SKILL_SUBTLETY:
                    case SKILL_BEAST_MASTERY:
                    case SKILL_SURVIVAL:
                    case SKILL_DEFENSE:
                    case SKILL_DUAL_WIELD:
                    case SKILL_FERAL_COMBAT:
                    case SKILL_PROTECTION:
                    case SKILL_PLATE_MAIL:
                    case SKILL_DEMONOLOGY:
                    case SKILL_ENHANCEMENT:
                    case SKILL_MAIL:
                    case SKILL_HOLY2:
                    case SKILL_LOCKPICKING:
                        break;

                    default: continue;
                    }
                }

                // skip wrong class/race skills
                if (!player->IsSpellFitByClassAndRace(tSpell->spell))
                    continue;

                // skip broken spells
                if (!SpellMgr::IsSpellValid(proto, player, false))
                    continue;

                if (tSpell->spell)
                {
                    bool learned = false;
                    for (int j = 0; j < 3; ++j)
                    {
                        if (proto->Effect[j] == SPELL_EFFECT_LEARN_SPELL)
                        {
                            uint32 learnedSpell = proto->EffectTriggerSpell[j];
                            player->learnSpell(learnedSpell, false);
                            learned = true;
                        }
                    }

                    if (!learned)
                    {
                        player->learnSpell(tSpell->spell, false);
                    }
                }
                else
                {
                    player->CastSpell(player, tSpell->spell, TRIGGERED_OLD_TRIGGERED);
                }
            }
        }

        player->PlaySpellVisual(249);
    }

    void BoostMgr::LearnWeaponSkills(Player* player)
    {
        // Learn weapon skills
        switch (player->getClass())
        {
        case CLASS_DRUID:
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_STAVES);
            SetMaxSkill(player, SKILL_2H_MACES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_FIST_WEAPONS);
#if BOOST_EXPANSION > 1
            SetMaxSkill(player, SKILL_POLEARMS);
#endif
            break;
        case CLASS_WARRIOR:
            SetMaxSkill(player, SKILL_SWORDS);
            SetMaxSkill(player, SKILL_AXES);
            SetMaxSkill(player, SKILL_BOWS);
            SetMaxSkill(player, SKILL_GUNS);
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_2H_SWORDS);
            SetMaxSkill(player, SKILL_STAVES);
            SetMaxSkill(player, SKILL_2H_MACES);
            SetMaxSkill(player, SKILL_2H_AXES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_CROSSBOWS);
            SetMaxSkill(player, SKILL_POLEARMS);
            SetMaxSkill(player, SKILL_FIST_WEAPONS);
            SetMaxSkill(player, SKILL_THROWN);
            break;
        case CLASS_PALADIN:
            SetMaxSkill(player, SKILL_SWORDS);
            SetMaxSkill(player, SKILL_AXES);
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_2H_SWORDS);
            SetMaxSkill(player, SKILL_2H_MACES);
            SetMaxSkill(player, SKILL_2H_AXES);
            SetMaxSkill(player, SKILL_POLEARMS);
            break;
        case CLASS_PRIEST:
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_STAVES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_WANDS);
            break;
        case CLASS_SHAMAN:
            SetMaxSkill(player, SKILL_AXES);
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_STAVES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_FIST_WEAPONS);
#if BOOST_EXPANSION > 0
            SetMaxSkill(player, SKILL_2H_MACES);
            SetMaxSkill(player, SKILL_2H_AXES);
#endif
            break;
        case CLASS_MAGE:
        case CLASS_WARLOCK:
            SetMaxSkill(player, SKILL_SWORDS);
            SetMaxSkill(player, SKILL_STAVES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_WANDS);
            break;
        case CLASS_HUNTER:
            SetMaxSkill(player, SKILL_SWORDS);
            SetMaxSkill(player, SKILL_AXES);
            SetMaxSkill(player, SKILL_BOWS);
            SetMaxSkill(player, SKILL_GUNS);
            SetMaxSkill(player, SKILL_2H_SWORDS);
            SetMaxSkill(player, SKILL_STAVES);
            SetMaxSkill(player, SKILL_2H_AXES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_CROSSBOWS);
            SetMaxSkill(player, SKILL_POLEARMS);
            SetMaxSkill(player, SKILL_FIST_WEAPONS);
            SetMaxSkill(player, SKILL_THROWN);
            break;
        case CLASS_ROGUE:
            SetMaxSkill(player, SKILL_SWORDS);
            SetMaxSkill(player, SKILL_AXES);
            SetMaxSkill(player, SKILL_BOWS);
            SetMaxSkill(player, SKILL_GUNS);
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_DAGGERS);
            SetMaxSkill(player, SKILL_CROSSBOWS);
            SetMaxSkill(player, SKILL_FIST_WEAPONS);
            SetMaxSkill(player, SKILL_THROWN);
            break;
#if BOOST_EXPANSION == 2
        case CLASS_DEATH_KNIGHT:
            SetMaxSkill(player, SKILL_SWORDS);
            SetMaxSkill(player, SKILL_AXES);
            SetMaxSkill(player, SKILL_MACES);
            SetMaxSkill(player, SKILL_2H_SWORDS);
            SetMaxSkill(player, SKILL_2H_MACES);
            SetMaxSkill(player, SKILL_2H_AXES);
            SetMaxSkill(player, SKILL_POLEARMS);
            break;
#endif
        }
    }

    void BoostMgr::LearnArmorSkills(Player* player)
    {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
        case CLASS_PALADIN:
            player->SetSkill(SKILL_PLATE_MAIL, 1, 1);
            break;
        case CLASS_SHAMAN:
        case CLASS_HUNTER:
            player->SetSkill(SKILL_MAIL, 1, 1);
            break;
        }
    }

    void BoostMgr::SetMaxSkill(Player* player, uint16 id)
    {
        SkillLineEntry const* sl = sSkillLineStore.LookupEntry(id);
        if (sl)
        {
            uint32 maxValue = 300;
            // do not let skill go beyond limit even if maxlevel > blizzlike
#if BOOST_EXPANSION > 0
            if (player->GetLevel() > 60)
            {
#if BOOST_EXPANSION == 1
                maxValue = (player->GetLevel() + 5) * 5;   // tbc (70 + 5)*5 = 375
#else
                maxValue = (player->GetLevel() + 10) * 5;  // wotlk (80 + 10)*5 = 450
#endif
            }
#endif

            uint32 maxPure = player->GetSkillMaxPure(id);
            if (maxPure > maxValue)
                maxValue = maxPure;

            if (player->GetSkillValue(id) < maxValue)
                player->SetSkill(id, maxValue, maxValue);
        }
    }

    void BoostMgr::CreatePet(Player* player, Creature* creature, uint32 entry)
    {
        if (Creature* creatureTarget = creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY() + 2, player->GetPositionZ(), player->GetOrientation(), TEMPSPAWN_CORPSE_TIMED_DESPAWN, 500))
        {
            creatureTarget->SetLevel(player->GetLevel() - 1);

            Pet* pet = new Pet(HUNTER_PET);

            if (!pet->CreateBaseAtCreature(creatureTarget))
            {
                delete pet;
                return;
            }

            pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, 13481); // tame beast

            pet->SetOwnerGuid(player->GetObjectGuid());
            pet->setFaction(player->GetFaction());

            if (player->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
            {
                pet->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
#if BOOST_EXPANSION > 0
                pet->SetByteValue(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_PLAYER_CONTROLLED_DEBUFF_LIMIT);
            }
            else
                pet->SetByteValue(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_CREATURE_DEBUFF_LIMIT);
#else
            }
#endif

            if (player->IsImmuneToNPC())
                pet->SetImmuneToNPC(true);

            if (player->IsImmuneToPlayer())
                pet->SetImmuneToPlayer(true);

            if (player->IsPvP())
                pet->SetPvP(true);

            pet->GetCharmInfo()->SetPetNumber(sObjectMgr.GeneratePetNumber(), true);

            uint32 level = creatureTarget->GetLevel();
            pet->SetCanModifyStats(true);
            pet->InitStatsForLevel(level);

            pet->SetHealthPercent(creatureTarget->GetHealthPercent());

            // destroy creature object
            creatureTarget->ForcedDespawn();

            // prepare visual effect for levelup
            pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);

            // add pet object to the world
            pet->GetMap()->Add((Creature*)pet);
            pet->AIM_Initialize();

            pet->AI()->SetReactState(REACT_DEFENSIVE);

            // visual effect for levelup
            pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

#if BOOST_EXPANSION > 0
            // enable rename and abandon prompt
            pet->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);
            pet->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_ABANDONED);
#endif

            // this enables pet details window (Shift+P)
            pet->InitPetCreateSpells();

            pet->LearnPetPassives();
            pet->CastPetAuras(true);
            pet->CastOwnerTalentAuras();
            pet->UpdateAllStats();

            // start with maximum loyalty and training points
            pet->SetLoyaltyLevel(BEST_FRIEND);
            pet->ModifyLoyalty(26500);
            pet->SetTP(level * (BEST_FRIEND - 1));
            pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2); // Content

            pet->SetRequiredXpForNextLoyaltyLevel();

            // caster have pet now
            player->SetPet(pet);

            player->PetSpellInitialize();

            pet->SavePetToDB(PET_SAVE_AS_CURRENT, player);
        }

        player->GetPlayerMenu()->CloseGossip();
        creature->MonsterWhisper("An excellent choice! May it serve you well on the battlefield.", player);
    }

    void BoostMgr::AddAmmo(Player* player)
    {
        if (player->getClass() != CLASS_HUNTER && player->getClass() != CLASS_ROGUE && player->getClass() != CLASS_WARRIOR)
            return;

        Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
        if (!pItem)
            return;

        uint32 subClass = 0;
        switch (pItem->GetProto()->SubClass)
        {
        case ITEM_SUBCLASS_WEAPON_GUN:
            subClass = ITEM_SUBCLASS_BULLET;
            break;
        case ITEM_SUBCLASS_WEAPON_BOW:
        case ITEM_SUBCLASS_WEAPON_CROSSBOW:
            subClass = ITEM_SUBCLASS_ARROW;
            break;
        case ITEM_SUBCLASS_WEAPON_THROWN:
            if (player->getClass() != CLASS_HUNTER)
            {
                subClass = ITEM_SUBCLASS_THROWN;
                break;
            }
        }

        if (!subClass)
            return;

        uint32 entry = player->GetUInt32Value(PLAYER_AMMO_ID);
        uint32 count = player->GetItemCount(entry) / 200;
        uint32 maxCount = 5 + player->GetLevel() / 10;

        if (!entry || count <= 2)
        {
#if BOOST_EXPANSION == 0
            entry = subClass == ITEM_SUBCLASS_ARROW ? 18042 : 15997;
#else
            entry = subClass == ITEM_SUBCLASS_ARROW ? 28053 : 28060;
#endif
            count = player->GetItemCount(entry) / 200;
        }

        if (!entry)
            return;

        if (count < maxCount)
        {
            for (uint32 i = 0; i < maxCount - count; i++)
            {
                Item* newItem = player->StoreNewItemInInventorySlot(entry, 200);
            }
        }

        if (player->GetUInt32Value(PLAYER_AMMO_ID) != entry)
            player->SetAmmo(entry);

        // add second type
        if (player->getClass() == CLASS_HUNTER)
        {
            subClass = subClass == ITEM_SUBCLASS_ARROW ? ITEM_SUBCLASS_BULLET : ITEM_SUBCLASS_ARROW;
#if BOOST_EXPANSION == 0
            entry = subClass == ITEM_SUBCLASS_ARROW ? 18042 : 15997;
#else
            entry = subClass == ITEM_SUBCLASS_ARROW ? 28053 : 28060;
#endif
            count = player->GetItemCount(entry) / 200;

            if (count < maxCount)
            {
                for (uint32 i = 0; i < maxCount - count; i++)
                {
                    Item* newItem = player->StoreNewItemInInventorySlot(entry, 200);
                }
            }
        }
    }

    void BoostMgr::AddFood(Player* player, uint32 count)
    {
        player->StoreNewItemInInventorySlot(20390, count);
    }

    void BoostMgr::AddReagents(Player* player)
    {
        std::list<uint32> items;
        uint32 regCount = 1;
        switch (player->getClass())
        {
        case CLASS_MAGE:
            regCount = 2;
            if (player->GetLevel() > 11)
                items = { 17056 };
            if (player->GetLevel() > 19)
                items = { 17056, 17031 };
            if (player->GetLevel() > 35)
                items = { 17056, 17031, 17032 };
            if (player->GetLevel() > 55)
                items = { 17056, 17031, 17032, 17020 };
            break;
        case CLASS_DRUID:
            regCount = 2;
            if (player->GetLevel() > 19)
                items = { 17034 };
            if (player->GetLevel() > 29)
                items = { 17035 };
            if (player->GetLevel() > 39)
                items = { 17036 };
            if (player->GetLevel() > 49)
                items = { 17037, 17021 };
            if (player->GetLevel() > 59)
                items = { 17038, 17026 };
            if (player->GetLevel() > 69)
                items = { 22147, 22148 };
            break;
        case CLASS_PALADIN:
            regCount = 3;
            if (player->GetLevel() > 50)
                items = { 21177 };
            break;
        case CLASS_SHAMAN:
            regCount = 1;
            if (player->GetLevel() > 22)
                items = { 17057 };
            if (player->GetLevel() > 28)
                items = { 17057, 17058 };
            if (player->GetLevel() > 29)
                items = { 17057, 17058, 17030 };
            break;
        case CLASS_WARLOCK:
            regCount = 10;
            if (player->GetLevel() > 9)
                items = { 6265 };
            if (player->GetLevel() > 49)
                items = { 6265, 5565 };
            break;
        case CLASS_PRIEST:
            regCount = 3;
            if (player->GetLevel() > 48)
                items = { 17028 };
            if (player->GetLevel() > 55)
                items = { 17028, 17029 };
            break;
        case CLASS_ROGUE:
            regCount = 1;
            if (player->GetLevel() > 21)
                items = { 5140 };
            if (player->GetLevel() > 33)
                items = { 5140, 5530 };
            break;
        }

        for (std::list<uint32>::iterator i = items.begin(); i != items.end(); ++i)
        {
            ItemPrototype const* proto = sObjectMgr.GetItemPrototype(*i);
            if (!proto)
                continue;

            uint32 maxCount = proto->GetMaxStackSize();
            uint32 count = player->GetItemCount(proto->ItemId);
            if (count > maxCount)
                continue;

            uint32 randCount = urand(maxCount / 2, maxCount * regCount);
            Item* newItem = player->StoreNewItemInInventorySlot(*i, randCount);
        }

        for (PlayerSpellMap::iterator itr = player->GetSpellMap().begin(); itr != player->GetSpellMap().end(); ++itr)
        {
            uint32 spellId = itr->first;

            if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled || IsPassiveSpell(spellId))
                continue;

            const SpellEntry* pSpellInfo = sSpellTemplate.LookupEntry<SpellEntry>(spellId);
            if (!pSpellInfo)
                continue;

            if (pSpellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL)
                continue;

            for (const auto& totem : pSpellInfo->Totem)
            {
                if (totem && !player->HasItemCount(totem, 1))
                {
                    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(totem);
                    if (!proto)
                        continue;

                    Item* newItem = player->StoreNewItemInInventorySlot(totem, 1);
                }
            }
#if BOOST_EXPANSION > 0
            for (const auto& totemCat : pSpellInfo->TotemCategory)
            {
                if (totemCat && !player->HasItemTotemCategory(totemCat))
                {
                    uint32 itemId = 0;
                    if (totemCat == 2)
                        itemId = 5175; // earth totem
                    if (totemCat == 3)
                        itemId = 5178; // air totem
                    if (totemCat == 4)
                        itemId = 5176; // fire totem
                    if (totemCat == 5)
                        itemId = 5177; // water totem

                    if (itemId)
                    {
                        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
                        if (!proto)
                            continue;

                        Item* newItem = player->StoreNewItemInInventorySlot(itemId, 1);
                    }
                }
            }
#endif
        }
    }

    bool BoostMgr::HasStarterSet(Player* player, const std::vector<uint32>& gearList)
    {
        uint8 counter = 0;
        for (auto item : gearList)
        {
            ItemPrototype const* proto = ObjectMgr::GetItemPrototype(item);
            if (!proto)
                continue;

            // don't check for quiver/ammo pouch
            if (proto->InventoryType == INVTYPE_BAG || item == 23197)
                continue;

            if (player->HasItemCount(item, 1))
                counter++;
        }

        return counter > 10;
    }

    void BoostMgr::RemoveStarterSet(Player* player, const std::vector<uint32>& gearList)
    {
        for (auto item : gearList)
        {
            ItemPrototype const* proto = ObjectMgr::GetItemPrototype(item);
            if (!proto)
                continue;

            // don't check for quiver/ammo pouch
            if (proto->InventoryType == INVTYPE_BAG)
                continue;

            player->DestroyItemCount(item, 2, true, false);
        }
    }

    void BoostMgr::AddStarterSet(Player* player, Creature* creature, uint32 setId, int32 suffixOverride, uint32 specId)
    {
        std::vector<uint32> gearList;

        if (setId == SET_ID_INSTANT_58)
        {
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedPhysical), std::end(Instant58SharedPhysical));
                gearList.insert(std::end(gearList), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
                gearList.insert(std::end(gearList), std::begin(Instant58Bow), std::end(Instant58Bow));
                gearList.insert(std::end(gearList), std::begin(Instant58DualWield), std::end(Instant58DualWield));
                gearList.insert(std::end(gearList), std::begin(Instant58PlateOffpieces), std::end(Instant58PlateOffpieces));
                gearList.insert(std::end(gearList), std::begin(Instant58Warrior), std::end(Instant58Warrior));
                break;
            case CLASS_PALADIN:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedPhysical), std::end(Instant58SharedPhysical));
                gearList.insert(std::end(gearList), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
                gearList.insert(std::end(gearList), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
                gearList.insert(std::end(gearList), std::begin(Instant58GavelAndShield), std::end(Instant58GavelAndShield));
                gearList.insert(std::end(gearList), std::begin(Instant58PlateOffpieces), std::end(Instant58PlateOffpieces));
                gearList.insert(std::end(gearList), std::begin(Instant58Paladin), std::end(Instant58Paladin));
                break;
            case CLASS_PRIEST:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
                gearList.insert(std::end(gearList), std::begin(Instant58Staff), std::end(Instant58Staff));
                gearList.insert(std::end(gearList), std::begin(Instant58ClothCaster), std::end(Instant58ClothCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58Priest), std::end(Instant58Priest));
                break;
            case CLASS_WARLOCK:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
                gearList.insert(std::end(gearList), std::begin(Instant58Staff), std::end(Instant58Staff));
                gearList.insert(std::end(gearList), std::begin(Instant58ClothCaster), std::end(Instant58ClothCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58Warlock), std::end(Instant58Warlock));
                break;
            case CLASS_MAGE:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
                gearList.insert(std::end(gearList), std::begin(Instant58Staff), std::end(Instant58Staff));
                gearList.insert(std::end(gearList), std::begin(Instant58ClothCaster), std::end(Instant58ClothCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58Mage), std::end(Instant58Mage));
                break;
            case CLASS_DRUID:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
                gearList.insert(std::end(gearList), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
                gearList.insert(std::end(gearList), std::begin(Instant58LeatherOffpieces), std::end(Instant58LeatherOffpieces));
                gearList.insert(std::end(gearList), std::begin(Instant58Staff), std::end(Instant58Staff));
                gearList.insert(std::end(gearList), std::begin(Instant58Druid), std::end(Instant58Druid));
                break;
            case CLASS_ROGUE:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedPhysical), std::end(Instant58SharedPhysical));
                gearList.insert(std::end(gearList), std::begin(Instant58Bow), std::end(Instant58Bow));
                gearList.insert(std::end(gearList), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
                gearList.insert(std::end(gearList), std::begin(Instant58LeatherOffpieces), std::end(Instant58LeatherOffpieces));
                gearList.insert(std::end(gearList), std::begin(Instant58Rogue), std::end(Instant58Rogue));
                break;
            case CLASS_SHAMAN:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedCaster), std::end(Instant58SharedCaster));
                gearList.insert(std::end(gearList), std::begin(Instant58SharedPhysical), std::end(Instant58SharedPhysical));
                gearList.insert(std::end(gearList), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
                gearList.insert(std::end(gearList), std::begin(Instant58MagicBack), std::end(Instant58MagicBack));
                gearList.insert(std::end(gearList), std::begin(Instant58MailOffpieces), std::end(Instant58MailOffpieces));
                gearList.insert(std::end(gearList), std::begin(Instant58GavelAndShield), std::end(Instant58GavelAndShield));
                gearList.insert(std::end(gearList), std::begin(Instant58DualWield), std::end(Instant58DualWield));
                gearList.insert(std::end(gearList), std::begin(Instant58Shaman), std::end(Instant58Shaman));
                break;
            case CLASS_HUNTER:
                gearList.insert(std::end(gearList), std::begin(Instant58SharedPhysical), std::end(Instant58SharedPhysical));
                gearList.insert(std::end(gearList), std::begin(Instant58PhysicalBack), std::end(Instant58PhysicalBack));
                gearList.insert(std::end(gearList), std::begin(Instant58Bow), std::end(Instant58Bow));
                gearList.insert(std::end(gearList), std::begin(Instant58MailOffpieces), std::end(Instant58MailOffpieces));
                gearList.insert(std::end(gearList), std::begin(Instant58Hunter), std::end(Instant58Hunter));
                break;
            }
        }
        else if (setId == SET_ID_PRIMARY)
        {
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
            case CLASS_PALADIN:
                gearList.insert(std::end(gearList), std::begin(PlatePrimaryGear), std::end(PlatePrimaryGear));
                break;
            case CLASS_PRIEST:
            case CLASS_WARLOCK:
            case CLASS_MAGE:
                gearList.insert(std::end(gearList), std::begin(ClothPrimaryGear), std::end(ClothPrimaryGear));
                break;
            case CLASS_DRUID:
            case CLASS_ROGUE:
                gearList.insert(std::end(gearList), std::begin(LeatherPrimaryGear), std::end(LeatherPrimaryGear));
                break;
            case CLASS_SHAMAN:
            case CLASS_HUNTER:
                gearList.insert(std::end(gearList), std::begin(MailPrimaryGear), std::end(MailPrimaryGear));
                break;
            }
        }
        else if (setId == SET_ID_SECONDARY)
        {
            gearList.insert(std::end(gearList), std::begin(SharedSecondaryGear), std::end(SharedSecondaryGear));
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
            case CLASS_PALADIN:
                gearList.insert(std::end(gearList), std::begin(PlateSecondaryGear), std::end(PlateSecondaryGear));
                break;
            case CLASS_PRIEST:
            case CLASS_WARLOCK:
            case CLASS_MAGE:
                gearList.insert(std::end(gearList), std::begin(ClothSecondaryGear), std::end(ClothSecondaryGear));
                break;
            case CLASS_DRUID:
            case CLASS_ROGUE:
                gearList.insert(std::end(gearList), std::begin(LeatherSecondaryGear), std::end(LeatherSecondaryGear));
                break;
            case CLASS_SHAMAN:
            case CLASS_HUNTER:
                gearList.insert(std::end(gearList), std::begin(MailSecondaryGear), std::end(MailSecondaryGear));
                break;
            }
        }
        else if (setId == SET_ID_TERTIARY)
        {
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
                if (specId == 0) gearList.insert(std::end(gearList), std::begin(ArmsWarriorTertiaryGear), std::end(ArmsWarriorTertiaryGear));
                else if (specId == 1) gearList.insert(std::end(gearList), std::begin(FuryWarriorTertiaryGear), std::end(FuryWarriorTertiaryGear));
                else if (specId == 2) gearList.insert(std::end(gearList), std::begin(ProtWarriorTertiaryGear), std::end(ProtWarriorTertiaryGear));
                break;
            case CLASS_PALADIN:
                if (specId == 0) gearList.insert(std::end(gearList), std::begin(HolyPaladinTertiaryGear), std::end(HolyPaladinTertiaryGear));
                else if (specId == 1) gearList.insert(std::end(gearList), std::begin(ProtPaladinTertiaryGear), std::end(ProtPaladinTertiaryGear));
                else if (specId == 2) gearList.insert(std::end(gearList), std::begin(RetPaladinTertiaryGear), std::end(RetPaladinTertiaryGear));
                break;
            case CLASS_PRIEST:
                if (specId == 0 || specId == 1) gearList.insert(std::end(gearList), std::begin(DiscHolyPriestTertiaryGear), std::end(DiscHolyPriestTertiaryGear));
                else if (specId == 2) gearList.insert(std::end(gearList), std::begin(CasterDPSTertiaryGear), std::end(CasterDPSTertiaryGear));
                break;
            case CLASS_WARLOCK:
            case CLASS_MAGE:
                gearList.insert(std::end(gearList), std::begin(CasterDPSTertiaryGear), std::end(CasterDPSTertiaryGear));
                break;
            case CLASS_DRUID:
                if (specId == 0) gearList.insert(std::end(gearList), std::begin(BalanceDruidTertiaryGear), std::end(BalanceDruidTertiaryGear));
                else if (specId == 1) gearList.insert(std::end(gearList), std::begin(FeralDruidTertiaryGear), std::end(FeralDruidTertiaryGear));
                else if (specId == 2) gearList.insert(std::end(gearList), std::begin(RestoDruidTertiaryGear), std::end(RestoDruidTertiaryGear));
                break;
            case CLASS_ROGUE:
                if (specId == 0) gearList.insert(std::end(gearList), std::begin(AssassinRogueTertiaryGear), std::end(AssassinRogueTertiaryGear));
                else if (specId == 1 || specId == 2) gearList.insert(std::end(gearList), std::begin(CombatSubRogueTertiaryGear), std::end(CombatSubRogueTertiaryGear));
                break;
            case CLASS_SHAMAN:
                if (specId == 0) gearList.insert(std::end(gearList), std::begin(ElementalShamanTertiaryGear), std::end(ElementalShamanTertiaryGear));
                else if (specId == 1) gearList.insert(std::end(gearList), std::begin(EnhanceShamanTertiaryGear), std::end(EnhanceShamanTertiaryGear));
                else if (specId == 2) gearList.insert(std::end(gearList), std::begin(RestoShamanTertiaryGear), std::end(RestoShamanTertiaryGear));
                break;
            case CLASS_HUNTER:
                gearList.insert(std::end(gearList), std::begin(HunterTertiaryGear), std::end(HunterTertiaryGear));
                break;
            }
        }

        // special effect
        player->PlaySpellVisual(83);

        GivePlayerItems(player, creature, gearList, setId, suffixOverride);
    }

    void BoostMgr::GivePlayerItems(Player* recipient, Creature* creature, std::vector<uint32> gearList, uint32 setId, int32 suffixOverride)
    {
        // Remove any gear the character still has from initial creation (now useless)
        for (uint32 itemEntry : Level1StartingGear)
            recipient->DestroyItemCount(itemEntry, 200, true, false);

        bool allSuccess = true;
        bool alreadyHave = false;
        for (auto item : gearList)
        {
            ItemPrototype const* proto = ObjectMgr::GetItemPrototype(item);
            if (!proto)
                continue;

            if (recipient->HasItemCount(item, (proto->InventoryType != INVTYPE_FINGER && proto->InventoryType != INVTYPE_WEAPON) ? 1 : 2))
            {
                alreadyHave = true;
                continue;
            }

            if (!recipient->StoreNewItemInBestSlots(item, 1/*, (proto->InventoryType == INVTYPE_TRINKET || proto->InventoryType == INVTYPE_RELIC || proto->InventoryType == INVTYPE_BAG) ? 0 : -suffixOverride*/))
                allSuccess = false;
        }

        if (alreadyHave)
            creature->MonsterWhisper("There are already some pieces of starter gear in your possession. You must destroy it first before I can give you more!", recipient);

        if (!allSuccess)
            creature->MonsterWhisper("You can't hold some of the gear I'm trying to give you! Make room for it first and speak to me again.", recipient);
        else if (!setId || setId == SET_ID_TERTIARY || setId == SET_ID_INSTANT_58)
            creature->MonsterWhisper("This equipment will serve you well in battle.", recipient);
    }

    void BoostMgr::OfferPrimarySecondaryModChoices(Player* player, uint32 actionIdBase, uint32 specId)
    {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            if (specId == 0 || specId == 1) // Arms/Fury
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Ancestor (Str, Crit, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 54);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Battle (Str, Stam, Crit)", GOSSIP_SENDER_MAIN, actionIdBase + 56);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Beast (Str, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 41);
            }
            else if (specId == 2) // Protection
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Blocking (Block, Str)", GOSSIP_SENDER_MAIN, actionIdBase + 47);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Champion (Str, Stam, Def)", GOSSIP_SENDER_MAIN, actionIdBase + 45);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Soldier (Str, Stam, Crit)", GOSSIP_SENDER_MAIN, actionIdBase + 43);
            }
            break;
        case CLASS_PALADIN:
            if (specId == 0) // Holy
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Physician (Stam, Int, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 37);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Vision (Spell Dmg/Heal, Int, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 53);
            }
            else if (specId == 1) // Protection
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Champion (Str, Stam, Def)", GOSSIP_SENDER_MAIN, actionIdBase + 45);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Crusade (Spell Dmg/Heal, Int, Def)", GOSSIP_SENDER_MAIN, actionIdBase + 52);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Knight (Stam, Spell Dmg/Heal, Def)", GOSSIP_SENDER_MAIN, actionIdBase + 66);
            }
            else if (specId == 2) // Retribution
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Ancestor (Str, Crit, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 54);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Beast (Str, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 41);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Soldier (Str, Stam, Crit)", GOSSIP_SENDER_MAIN, actionIdBase + 43);
            }
            break;
        case CLASS_PRIEST:
            if (specId == 0 || specId == 1) // Discipline/Holy
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Physician (Stam, Int, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 37);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Vision (Spell Dmg/Heal, Int, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 53);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Prophet (Int, Spirit, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 38);
            }
            else if (specId == 2) // Shadow
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Nightmare (Shadow Dmg, Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 55);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
            }
            break;
        case CLASS_DRUID:
            if (specId == 0) // Balance
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Vision (Spell Dmg/Heal, Int, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 53);
            }
            else if (specId == 1) // Feral
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bandit (Agi, Stam, Attack Power)", GOSSIP_SENDER_MAIN, actionIdBase + 40);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Beast (Str, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 41);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Champion (Str, Stam, Def)", GOSSIP_SENDER_MAIN, actionIdBase + 45);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shadow (Attack Power, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 57);
            }
            else if (specId == 2) // Restoration
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Hierophant (Stam, Spirit, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 42);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Prophet (Int, Spirit, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 38);
            }
            break;
        case CLASS_SHAMAN:
            if (specId == 0) // Elemental
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Invoker (Int, Spell Dmg/Heal, Spell Crit)", GOSSIP_SENDER_MAIN, actionIdBase + 39);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Vision (Spell Dmg/Heal, Int, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 53);
            }
            else if (specId == 1) // Enhancement
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Ancestor (Str, Crit, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 54);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Beast (Str, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 41);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Hunt (Attack Power, Agil, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 50);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Wild (Attack Power, Stam, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 60);
            }
            else if (specId == 2) // Restoration
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Elder (Stam, Int, Mana Regen)", GOSSIP_SENDER_MAIN, actionIdBase + 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Physician (Stam, Int, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 37);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Prophet (Int, Spirit, Spell Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 38);
            }
            break;
        case CLASS_WARLOCK: // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Invoker (Int, Spell Dmg/Heal, Spell Crit)", GOSSIP_SENDER_MAIN, actionIdBase + 39);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Vision (Spell Dmg/Heal, Int, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 53);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Nightmare (Shadow Dmg, Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 55);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Sorcerer (Stam, Int, Spell Dmg/Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 36);
            break;
        case CLASS_ROGUE:   // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bandit (Agi, Stam, Attack Power)", GOSSIP_SENDER_MAIN, actionIdBase + 40);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shadow (Attack Power, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 57);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Wild (Attack Power, Stam, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 60);
            break;
        case CLASS_HUNTER:  // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Hunt (Attack Power, Agil, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 50);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bandit (Agil, Stam, Attack Power)", GOSSIP_SENDER_MAIN, actionIdBase + 40);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shadow (Attack Power, Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 57);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Wild (Attack Power, Stam, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 60);
            break;
        case CLASS_MAGE:    // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Invoker (Int, Spell Dmg/Heal, Spell Crit)", GOSSIP_SENDER_MAIN, actionIdBase + 39);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Mind (Spell Dmg/Heal, Spell Crit, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 51);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Vision (Spell Dmg/Heal, Int, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 53);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Sorcerer (Stam, Int, Spell Dmg/Heal)", GOSSIP_SENDER_MAIN, actionIdBase + 36);
            break;
        }
    }

    void BoostMgr::OfferTertiaryModChoices(Player* player, uint32 actionIdBase, uint32 specId)
    {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            if (specId == 0 || specId == 1) // Arms/Fury
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Tiger (Str, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bear (Str, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Boar (Str, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 12);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
            }
            else if (specId == 2) // Protection
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Tiger (Str, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bear (Str, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Boar (Str, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 12);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
            }
            break;
        case CLASS_PALADIN:
            if (specId == 0) // Holy
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Owl (Int, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            }
            else if (specId == 1) // Protection
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bear (Str, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Gorilla (Str, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
            }
            else if (specId == 2) // Retribution
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bear (Str, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Gorilla (Str, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Tiger (Str, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
            }
            break;
        case CLASS_PRIEST: // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Owl (Int, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 9);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Whale (Stam, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 8);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Spirit", GOSSIP_SENDER_MAIN, actionIdBase + 15);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            break;
        case CLASS_DRUID:
            if (specId == 0) // Balance
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Whale (Stam, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 8);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Spirit", GOSSIP_SENDER_MAIN, actionIdBase + 15);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            }
            else if (specId == 1) // Feral
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Bear (Str, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Monkey (Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 5);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Tiger (Str, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
            }
            else if (specId == 2) // Restoration
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Whale (Stam, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 8);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Spirit", GOSSIP_SENDER_MAIN, actionIdBase + 15);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            }
            break;
        case CLASS_SHAMAN:
            if (specId == 0) // Elemental
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Owl (Int, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            }
            else if (specId == 1) // Enhancement
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Falcon (Agil, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 11);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Monkey (Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 5);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Tiger (Str, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Gorilla (Str, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Wolf (Agil, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
            }
            else if (specId == 2) // Restoration
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Owl (Int, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Spirit", GOSSIP_SENDER_MAIN, actionIdBase + 15);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            }
            break;
        case CLASS_WARLOCK: // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Whale (Stam, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 8);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            break;
        case CLASS_ROGUE:   // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Monkey (Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 5);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Tiger (Str, Agil)", GOSSIP_SENDER_MAIN, actionIdBase + 14);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Strength", GOSSIP_SENDER_MAIN, actionIdBase + 17);
            break;
        case CLASS_HUNTER:  // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Monkey (Agil, Stam)", GOSSIP_SENDER_MAIN, actionIdBase + 5);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Falcon (Agil, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 11);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Wolf (Agil, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 13);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Agility", GOSSIP_SENDER_MAIN, actionIdBase + 18);
            break;
        case CLASS_MAGE:    // All specs
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Eagle (Stam, Int)", GOSSIP_SENDER_MAIN, actionIdBase + 6);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Owl (Int, Spirit)", GOSSIP_SENDER_MAIN, actionIdBase + 9);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Spirit", GOSSIP_SENDER_MAIN, actionIdBase + 15);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Intellect", GOSSIP_SENDER_MAIN, actionIdBase + 19);
            break;
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Stamina", GOSSIP_SENDER_MAIN, actionIdBase + 16); // all classes/specs may opt for this here
    }

    bool BoostMgr::TaxiNodesKnown(Player& player) const
    {
        switch (uint32(player.GetTeam()))
        {
        case ALLIANCE:
            for (uint32 node : Instant58TaxiNodesAlliance)
            {
                if (!player.m_taxi.IsTaximaskNodeKnown(node))
                    return false;
            }
            break;
        case HORDE:
            for (uint32 node : Instant58TaxiNodesHorde)
            {
                if (!player.m_taxi.IsTaximaskNodeKnown(node))
                    return false;
            }
            break;
        }
        return true;
    }

    void BoostMgr::TaxiNodesTeach(Player& player) const
    {
        // Vanilla taxi nodes up to 2.0.1 progression patch

        switch (uint32(player.GetTeam()))
        {
        case ALLIANCE:
            for (uint32 node : Instant58TaxiNodesAlliance)
                player.m_taxi.SetTaximaskNode(node);
            break;
        case HORDE:
            for (uint32 node : Instant58TaxiNodesHorde)
                player.m_taxi.SetTaximaskNode(node);
            break;
        }

        player.PlaySpellVisual(248);
    }   
}
