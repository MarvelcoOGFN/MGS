#pragma once
#include "Utils.h"
#include "Gamemode.h"
#include "PC.h"
#include "Tick.h"
#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

static auto ImageBase = InSDKUtils::GetImageBase();

int True() {
	return 1;
}

int False() {
	return 0;
}


namespace Gamemode {

	inline void InitHooks()
	{
		Log("Hooking Gamemode");

		MH_CreateHook((LPVOID)(ImageBase + OFFSET), ReadyToStartMatch, (LPVOID*)&ReadyToStartMatchOG);
		MH_CreateHook((LPVOID)(ImageBase + OFFSET), SpawnDefaultPawnFor, nullptr);
		MH_CreateHook(LPVOID(ImageBase + OFFSET), PickTeam, nullptr);
	}
}

namespace Misc {

	void InitHooks()
	{
		Log("Hooking Misc");

		MH_CreateHook((LPVOID)(ImageBase + OFFSET), True, nullptr); // collect garbage
		MH_CreateHook((LPVOID)(ImageBase + OFFSET), False, nullptr);// validation / kick
		MH_CreateHook((LPVOID)(ImageBase + OFFSET), False, nullptr);// change gamesession id
		MH_CreateHook((LPVOID)(ImageBase + OFFSET), DispatchRequest, (LPVOID*)&DispatchRequestOG);
		MH_CreateHook((LPVOID)(ImageBase + OFFSET), AActorGetNetMode, nullptr);
		MH_CreateHook((LPVOID)(ImageBase + OFFSET), WorldGetNetMode, nullptr);

	}
}

namespace PC {

	void InitHooks()
	{
		Log("Hooking PC");

		HookVTable(AFortPlayerControllerAthena::GetDefaultObj(), INDEX, ServerLoadingScreenDropped, (LPVOID*)&ServerLoadingScreenDroppedOG);

		HookVTable(UFortControllerComponent_Aircraft::GetDefaultObj(), INDEX, ServerAttemptAircraftJump, nullptr);

		HookVTable(AFortPlayerControllerAthena::GetDefaultObj(), INDEX, ServerAcknowledgePossession, (LPVOID*)&ServerAcknowledgePossessionOG);
	}
}

namespace Tick {

	void InitHooks()
	{
		Log("Hooking Tick");

		MH_CreateHook((LPVOID)(ImageBase + OFFSET), TickFlush, (LPVOID*)&TickFlushOG);

		MH_CreateHook((LPVOID)(ImageBase + OFFSET), GetMaxTickRate, nullptr);
	}
}
