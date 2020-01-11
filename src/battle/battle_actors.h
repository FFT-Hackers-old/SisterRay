#ifndef BATTLE_ACTORS_H
#define BATTLE_ACTORS_H

#include <SisterRay/SisterRay.h>
#include "../party/stat_boosts.h"
#include "../sr_named_registry.h"
#include <map>
#include <array>

typedef struct {
    u8 buffer[1024]; //this is 8x the amount available in the base game, per actor
} ActorScriptVariables;

//Maintained for each of the games 10 actors. 
typedef struct {
    std::map<std::string, SrBattleStat> battleStats;
    ActorBattleVars gameAIState;
    ActorTimerData gameTimerState;
} SrBattleActor;

typedef struct {
    SrBattleActor battleActor;
    BattleParty34 party34;
    BattleParty10 party10;
} SrPartyBattleActor;

typedef struct {
    SrBattleActor battleActor;
} SrEnemyBattleActor;

typedef struct {
    ActorTimerData* actorTimers;
    ActorBattleVars* actorBattleVars;
    std::map<std::string, SrBattleStat>* battleStats;
    BattleParty34* party34; //null for enemy actors
    BattleParty10* party10; //null for enemy actors
} ActorBattleState;


class SrBattleActors {
public:
    SrBattleActors();
    ActorBattleState getActiveBattleActor(u8 actorIdx);
    void setPartyMemberActive(u8 partyIdx, u8 characterID);
    u8* getActorRandomVarBuffer(u8 actorIdx);
    void swapPartyActors(u8 partyIdx, u8 newCharacterID);
protected:
    std::array<u8, 3> activeParty;
    std::array<SrPartyBattleActor, 10> partyActors;
    std::array<SrEnemyBattleActor, 6> enemyActors;
    std::array<ActorScriptVariables, 10> actorScriptVariables;
};

const SrBattleStat& getSrBattleStat(u8 actorID, std::string statName);
SrBattleStat& getMutableSrBattleStat(u8 actorID, std::string statName);
void setActorBattleStat(u8 actorID, std::string statName, u32 value);

ActorBattleVars* getActorBattleVars(u8 actorIdx);
ActorTimerData* getActorTimerBlock(u8 actorIdx);

#endif
