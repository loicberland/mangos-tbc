#pragma once

#include "Entities/Player.h"

#include <map>
#include <string>
#include <unordered_map>

class Creature;
class Item;

#define MAX_TALENT_RANK 5
#define MAX_TALENT_SPECS 2

#define DUALSPEC_NPC_ENTRY 100601
#define DUALSPEC_ITEM_ENTRY 17731

#define DUALSPEC_NPC_TEXT 50700
#define DUALSPEC_ITEM_TEXT 50701

enum DualSpecMessages
{
    DUAL_SPEC_DESCRIPTION = 12000,
    DUAL_SPEC_COST_IS,
    DUAL_SPEC_CHANGE_MY_SPEC,
    DUAL_SPEC_NO_GOLD_UNLOCK,
    DUAL_SPEC_ARE_YOU_SURE_BEGIN,
    DUAL_SPEC_ARE_YOU_SURE_END,
    DUAL_SPEC_ALREADY_ON_SPEC,
    DUAL_SPEC_ACTIVATE,
    DUAL_SPEC_RENAME,
    DUAL_SPEC_UNNAMED,
    DUAL_SPEC_ACTIVE,
    DUAL_SPEC_ERR_COMBAT,
    DUAL_SPEC_ERR_INSTANCE,
    DUAL_SPEC_ERR_MOUNT,
    DUAL_SPEC_ERR_DEAD,
    DUAL_SPEC_ERR_UNLOCK,
    DUAL_SPEC_ERR_LEVEL,
    DUAL_SPEC_ACTIVATE_COLOR,
    DUAL_SPEC_RENAME_COLOR,
    DUAL_SPEC_ARE_YOU_SURE_SWITCH,
    DUAL_SPEC_PURCHASE,
    DUAL_SPEC_ERR_ITEM_CREATE,
};

struct DualspecPlayerTalent
{
    uint8 state;
    uint8 spec;
};

struct DualspecPlayerStatus
{
    uint8 specCount;
    uint8 activeSpec;
};

typedef std::unordered_map<uint32, DualspecPlayerTalent> DualSpecPlayerTalentMap;

class DualSpecMgr
{
public:
    static DualSpecMgr& Instance();

    void Initialize();

    bool OnUseItem(Player* player, Item* item);
    bool OnPreGossipHello(Player* player, Creature* creature);

    bool OnGossipSelect(
        Player* player,
        Creature* creature,
        uint32 sender,
        uint32 action,
        const std::string& code,
        uint32 gossipListId);

    bool OnGossipSelect(
        Player* player,
        Item* item,
        uint32 sender,
        uint32 action,
        const std::string& code,
        uint32 gossipListId);

    void OnLearnTalent(Player* player, uint32 spellId);
    void OnResetTalents(Player* player, uint32 cost);

    void OnPreLoadFromDB(uint32 playerId);
    void OnLoadFromDB(Player* player);
    void OnSaveToDB(Player* player);
    void OnDeleteFromDB(uint32 playerId);

    void OnLogOut(Player* player);
    void OnCharacterCreated(Player* player);

    bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons);
    bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons);

private:
    DualSpecMgr() = default;
    ~DualSpecMgr() = default;

    DualSpecMgr(const DualSpecMgr&) = delete;
    DualSpecMgr& operator=(const DualSpecMgr&) = delete;

    void LoadPlayerSpec(uint32 playerId);
    uint8 GetPlayerActiveSpec(uint32 playerId) const;
    void SetPlayerActiveSpec(Player* player, uint8 spec);

    uint8 GetPlayerSpecCount(uint32 playerId) const;
    void SetPlayerSpecCount(Player* player, uint8 count);
    void SavePlayerSpec(uint32 playerId);

    void LoadPlayerSpecNames(Player* player);
    const std::string& GetPlayerSpecName(Player* player, uint8 spec) const;
    void SetPlayerSpecName(Player* player, uint8 spec, const std::string& name);
    void SavePlayerSpecNames(Player* player);

    void LoadPlayerTalents(Player* player);
    bool PlayerHasTalent(Player* player, uint32 spellId, uint8 spec);

    DualSpecPlayerTalentMap* GetPlayerTalents(
        uint32 playerId,
        int8 spec = -1,
        bool assert = true);

    void AddPlayerTalent(
        uint32 playerId,
        uint32 spellId,
        uint8 spec,
        bool learned);

    void SavePlayerTalents(uint32 playerId);

    void SendPlayerActionButtons(const Player* player, bool clear) const;
    void ActivatePlayerSpec(Player* player, uint8 spec);
    void AddDualSpecItem(Player* player);

private:
    std::map<uint32, DualSpecPlayerTalentMap[MAX_TALENT_SPECS]> playersTalents;
    std::map<uint32, std::string[MAX_TALENT_SPECS]> playersSpecNames;
    std::map<uint32, DualspecPlayerStatus> playersStatus;
};

#define sDualSpecMgr DualSpecMgr::Instance()