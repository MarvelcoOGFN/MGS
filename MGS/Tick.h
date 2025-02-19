#pragma once
#include "Utils.h"

inline void (*TickFlushOG)(UNetDriver* Driver, float DeltaSeconds);
inline void TickFlush(UNetDriver* Driver, float DeltaSeconds)
{
	static void (*ServerReplicateActors)(void*) = decltype(ServerReplicateActors)(__int64(GetModuleHandleW(0)) + 0x1023F60);
	if (ServerReplicateActors && Driver->ReplicationDriver)
		ServerReplicateActors(Driver->ReplicationDriver);


	return TickFlushOG(Driver, DeltaSeconds);
}

inline float GetMaxTickRate() 
{
	return 30.f;
}