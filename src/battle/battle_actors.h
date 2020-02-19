#ifndef BATTLE_ACTORS_H
#define BATTLE_ACTORS_H

#include <SisterRay/SisterRay.h>
#include "../party/battle_stats.h"
#include "../sr_named_registry.h"
#include <map>
#include <array>

typedef struct {
    u8 buffer[1024]; //this is 8x the amount available in the base game, per actor
} ActorScriptVariables;

//There can be multiple instances of a status, unlike the original game, which can change effects
typedef struct {
    std::string statusName;
    u16 ticksToExpiration;
    u16 initialDuration;
    u16 stacks;
    u16 maxStacks;
    u16 maxInstances;
    u32 context[8]; // 8 variables to use for each status to track important things
    std::vector<std::string> tags;
} ActiveStatus;

//Maintained for each of the games 10 actors. 
typedef struct {
    std::map<std::string, SrBattleStat> battleStats;
    ActorBattleVars gameAIState;
    ActorTimerData gameTimerState;
    std::vector<ActiveStatus> activeStatuses;
} SrBattleActor;

typedef struct {
    SrBattleActor battleActor;
    BattleParty34 party34;
    BattleParty10 party10;
    BattleWeaponCtx weaponCtx;
} SrPartyBattleActor;

typedef struct {
    SrBattleActor battleActor;
} SrEnemyBattleActor;

struct ActorBattleState_{
    ActorTimerData* actorTimers;
    ActorBattleVars* actorBattleVars;
    std::map<std::string, SrBattleStat>* battleStats;
    BattleParty34* party34; //null for enemy actors
    BattleParty10* party10; //null for enemy actors
    BattleWeaponCtx* weaponCtx;
    std::vector<ActiveStatus>* activeStatuses;
};


class SrBattleActors {
public:
    SrBattleActors();
    ActorBattleState getSrBattleActor(u8 actorIdx);
    ActorBattleState getActiveBattleActor(u8 actorIdx);
    void initializePartyActor(u8 partyIdx, u8 characterID);
    void initializeEnemyActor(u8 actorIdx);
    void swapPartyActors(u8 partyIdx, u8 newCharacterID);
    u8* getActorRandomVarBuffer(u8 actorIdx);
protected:
    void setActivePartyActor(u8 partyIdx, u8 characterID);
    void activatePartyActor(u8 partyIdx);
    void activateEnemyActor(u8 enemyIdx);
    void savePartyActor(u8 partyIdx);
    void saveEnemyActor(u8 enemyIdx);
private:
    std::array<u8, 3> activeParty;
    std::array<SrPartyBattleActor, 10> partyActors;
    std::array<SrEnemyBattleActor, 6> enemyActors;
    std::array<ActorScriptVariables, 10> actorScriptVariables;

};

const SrBattleStat& getSrBattleStat(u8 actorID, std::string statName);
SrBattleStat& getMutableSrBattleStat(u8 actorID, std::string statName);
void setActorBattleStat(u8 actorID, std::string statName, i32 value);
void setPartyStats(u8 partyIdx, ActorBattleState& partyActor);
void setWeaponData(u8 partyIdx, ActorBattleState& partyActor);
void setEnemyStats(u8 enemyIndex, ActorBattleState& partyActor);

ActorBattleVars* getActorBattleVars(u8 actorIdx);
ActorTimerData* getActorTimerBlock(u8 actorIdx);

void initializePlayerActors();
void initializeEnemyActors();

ActorBattleVars* getActorBattleVars(u8 actorIdx);
BattleParty34* getBattleParty34(u8 actorIdx);
BattleParty10* getBattleParty10(u8 actorIdx);
BattleWeaponCtx* getBattleWeaponCtx(u8 actorIdx);
const SrBattleStat& getSrBattleStat(u8 actorID, std::string statName);
SrBattleStat& getMutableSrBattleStat(u8 actorID, std::string statName);
CharacterRecord* getCharacterRecordWithID(u8 characterID);


#endif
