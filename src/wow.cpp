#include <wow.h>

int CWow::ClntObjMgrGetActivePlayer(void)
{
	return 0;
	return ((int (*)(void))(baseaddr + 0x4FC6))();
}

unsigned short CWow::GetGameBuild(void)
{
	return *(unsigned short*)((baseaddr + GAMEBUILD_OFF));
}

uintptr_t CWow::GetPlayerPointer(void)
{
	return *(uintptr_t*)(baseaddr + PLAYERPTR_OFF);
}

bool CWowPlayer::IsInGame(void)
{
	return CWow::GetPlayerPointer() != 0;
}

bool CWowPlayer::IsMounted(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uint8_t* m = (uint8_t*)(pbase + PLAYER_ISMOUNTED);
	return (*m);
}

bool CWowPlayer::IsFishing(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uint8_t* f2 = (uint8_t*)(pbase + PLAYER_ISFISHING2);
	uint8_t* f3 = (uint8_t*)(pbase + PLAYER_ISFISHING3);
	return (*f2 && *f3);
}
