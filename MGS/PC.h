#pragma once
#include "Utils.h"


inline void (*ServerLoadingScreenDroppedOG)(AFortPlayerControllerAthena* PC);
inline void ServerLoadingScreenDropped(AFortPlayerControllerAthena* PC)
{
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	AFortGameStateAthena* GameState = (AFortGameStateAthena*)UWorld::GetWorld()->GameState;

	UFortKismetLibrary::UpdatePlayerCustomCharacterPartsVisualization(PlayerState);
	PlayerState->OnRep_CharacterData();


	PlayerState->SquadId = PlayerState->TeamIndex - 2;

	PlayerState->OnRep_SquadId();
	PlayerState->OnRep_TeamIndex(0);
	PlayerState->OnRep_PlayerTeam();
	PlayerState->OnRep_PlayerTeamPrivate();

	FGameMemberInfo Info{ -1,-1,-1 };
	Info.MemberUniqueId = PlayerState->UniqueId;
	Info.SquadId = PlayerState->SquadId;
	Info.TeamIndex = PlayerState->TeamIndex;

	GameState->GameMemberInfoArray.Members.Add(Info);
	GameState->GameMemberInfoArray.MarkItemDirty(Info);

	PC->XPComponent->bRegisteredWithQuestManager = true;
	PC->XPComponent->OnRep_bRegisteredWithQuestManager();

	return ServerLoadingScreenDroppedOG(PC);
}


inline void ServerAttemptAircraftJump(UFortControllerComponent_Aircraft* Comp, FRotator Rotation)
{
	auto PC = (AFortPlayerControllerAthena*)Comp->GetOwner();
	UWorld::GetWorld()->AuthorityGameMode->RestartPlayer(PC);

	if (PC->MyFortPawn)
		((AFortPlayerPawnAthena*)PC->MyFortPawn)->BeginSkydiving(true);
}

void (*ServerAcknowledgePossessionOG)(AFortPlayerControllerAthena* PC, APawn* Pawn);
inline void ServerAcknowledgePossession(AFortPlayerControllerAthena* PC, APawn* Pawn)
{
	PC->AcknowledgedPawn = Pawn;
	return ServerAcknowledgePossessionOG(PC, Pawn);
}

