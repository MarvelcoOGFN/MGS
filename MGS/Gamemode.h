#pragma once
#include "Utils.h"

inline bool (*ReadyToStartMatchOG)(AFortGameModeAthena* GameMode);
inline bool ReadyToStartMatch(AFortGameModeAthena* GameMode)
{
    ReadyToStartMatchOG(GameMode);

    auto GameState = (AFortGameStateAthena*)GameMode->GameState;

    static bool Initialized = false;
    static bool Listening = false;

    if (!GameMode || !GameState)
    {
        return false;
    }

    if (!Initialized)
    {
        Initialized = true;

        UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");

        GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
        GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
        GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
        GameState->CurrentPlaylistId = Playlist->PlaylistId;
        GameState->CurrentPlaylistInfo.MarkArrayDirty(); // make sure you add this to your sdk
        
        AFortGameSessionDedicatedAthena* GameSession = SpawnActor<AFortGameSessionDedicatedAthena>();

        GameSession->SessionName = UKismetStringLibrary::Conv_StringToName(FString(L"MGS"));
        GameSession->MaxPlayers = 100;
        GameMode->GameSession = GameSession;
        GameMode->FortGameSession = GameSession;
        GameMode->CurrentPlaylistId = Playlist->PlaylistId;
    }

    if (!GameState->MapInfo)
    {
        return false;
    }

    if (!Listening)
    {
        Listening = true;

        if (Initialized)
        {
            GameState->OnRep_CurrentPlaylistId();
            GameState->OnRep_CurrentPlaylistInfo();
        }


        FName GameNetDriver = UKismetStringLibrary::Conv_StringToName(FString(L"GameNetDriver"));

        UNetDriver* Driver = CreateNetDriver(UEngine::GetEngine(), UWorld::GetWorld(), GameNetDriver);

        Driver->World = UWorld::GetWorld();
        Driver->NetDriverName = GameNetDriver;

        FString Error;
        FURL URL = FURL();
        URL.Port = 7777;

        InitListen(Driver, UWorld::GetWorld(), URL, false, Error);
        SetWorld(Driver, UWorld::GetWorld());

        UWorld::GetWorld()->NetDriver = Driver;
        UWorld::GetWorld()->LevelCollections[0].NetDriver = Driver;
        UWorld::GetWorld()->LevelCollections[1].NetDriver = Driver;

        GameMode->bWorldIsReady = true;
        GameMode->WarmupRequiredPlayerCount = 1;

        SetConsoleTitleA("MGS hosting on port 7777");
        Log("Listening on port 7777.");
    }

    return UWorld::GetWorld()->NetDriver->ClientConnections.Num() > 0;
}


inline APawn* SpawnDefaultPawnFor(AFortGameMode* GameMode, AFortPlayerController* Player, AActor* StartingLoc)
{
    FTransform Transform = StartingLoc->GetTransform();
    return (AFortPlayerPawnAthena*)GameMode->SpawnDefaultPawnAtTransform(Player, Transform);
}

inline void (*DispatchRequestOG)(__int64 a1, unsigned __int64* a2, int a3);
inline void DispatchRequest(__int64 a1, unsigned __int64* a2, int a3)
{
    return DispatchRequestOG(a1, a2, 3);
}

inline __int64 PickTeam(__int64 a1, unsigned __int8 a2, __int64 a3)
{
    return 3;
};

