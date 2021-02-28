#pragma once
#include "geom.h"

/*
Render Scope 0x463CEB
Admin Hack 0x220
bIdleBots 0x4FF722
CrossHairID 0x501C38
bEditMode 0x50A1AB

isoccluded 0x455D90
rendermodel 0x414350
renderclient 0x414F20
renderclientp 0x4154F0
renderclients 0x4157D0
*/

struct weapon;

class CEntity {
public:

	vec GetHeadPosition() {
		return *(vec*)(this + 0x4);
	}

	vec GetVelocity() {
		return *(vec*)(this + 0x10);
	}

	vec GetPosition() {
		return *(vec*)(this + 0x34);
	}

	vec GetViewAngles() {
		return *(vec*)(this + 0x40);
	}

	void SetViewAngles(vec angles) {
		*(vec*)(this + 0x40) = angles;
	}

	bool IsInWater(void) {
		return *(bool*)(this + 0x68);
	}

	bool IsOnFloor(void) {
		return *(bool*)(this + 0x69);
	}

	bool IsOnLadder(void) {
		return *(bool*)(this + 0x6A);
	}

	bool IsCrouching(void) {
		return (*(bool*)(this + 0x6C) || *(bool*)(this + 0x6D));
	}

	bool IsScoping(void) {
		return *(bool*)(this + 0x71);
	}

	int GetHealth(void) {
		return *(int*)(this + 0xF8);
	}

	int GetArmor(void) {
		return *(int*)(this + 0xFC);
	}

	int GetID(void) {
		return *(char*)(this + 0x1E4);
	}

	bool IsShooting(void) {
		return *(bool*)(this + 0x224);
	}
	void SetShootState(bool bState) {
		*(bool*)(this + 0x224) = bState;
	}

	char* GetName(void) {
		return (char*)(this + 0x225);
	}

	char GetTeam(void) {
		return *(char*)(this + 0x32C);
	}

	weapon* GetWeaponHandle(void) {
		return (weapon*)(this + 0x374);
	}
};

struct weapon
{
	int __vft;
	int type;
	CEntity* owner;
	int* guninfo;
	int* ammo;
	int* mag;
	int* gunwait;
	int shots;
	int reloading;
};

class CEntityList {
public:
	CEntity* ent[32];
};

class CGame {
public:
	CEntity* local;
	CEntityList* list;
	
	static int GetPlayerCount(void) {
		return *(int*)(0x50F4F4 + 0xC);
	}
	static CGame* GetInstance(void) {
		return (CGame*)0x50F4F4;
	}

	static int GetGameMode(void) {
		return *(int*)(0x50F49C);
	}
	static int GetLastMilis(void) {
		return *(int*)0x509EAC;
	}

	static bool IsTeamMode(void) {
		int* gameMode = (int*)0x50F49C;
		return (*gameMode == 0 || *gameMode == 4 || *gameMode == 5 || *gameMode == 7 || *gameMode == 11 || *gameMode == 13 || *gameMode == 14 || *gameMode == 16 || *gameMode == 17 || *gameMode == 20 || *gameMode == 21);
	}

	static bool IsBotMode(void) {
		int* gameMode = (int*)0x50F49C;
		return (*gameMode == 7 || *gameMode == 8 || *gameMode == 12 || (*gameMode >= 18 && *gameMode <= 21));
	}

	static bool IsFlagMode(void) {
		int* gameMode = (int*)0x50F49C;
		return (*gameMode == 5 || *gameMode == 13 || *gameMode == 14 || *gameMode == 15);
	}
};

enum GameModes {
	TEAMDEATHMATCH = 0,
	COOPEDIT,
	DEATHMATCH,
	SURVIVOR,
	TEAMSURVIVOR,
	CTF,
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
	LASTSWISSSTANDING,
	ONESHOTONEKILL,
	TEAMONESHOTONEKILL,
	BOTONESHOTONEKILL,
	HUNTTHEFLAG,
	TEAMKEEPTHEFLAG,
	KEEPTHEFLAG,
	TEAMPF,
	TEAMLSS,
	BOTPISTOLFRENZY,
	BOtlSS,
	BOTTEAMSURVIVOR,
	BOTTEAMONESHOTONEKILL,
	NUM
};

struct traceresult_s
{
	vec end;
	bool collided;
};

void TraceLine(vec from, vec to, CEntity* pTracer, bool CheckPlayers, traceresult_s* tr, bool SkipTags) {

	DWORD traceLine = 0x48A310;

	__asm
	{
		push SkipTags
		push CheckPlayers
		push pTracer
		push to.z
		push to.y
		push to.x
		push from.z
		push from.y
		push from.x
		mov eax,tr
		call traceLine
		add esp,0x24
	}
}

class CScreenSettings {
public:
	__int32 m_Width;
	__int32 m_Height;
	char _0x0008[8];
	__int32 m_GraphicDetail;
	__int32 m_AntiAlias;
	__int32 m_VSync;
	bool m_Fullscreen;
	char _0x001D[23];
	__int32 m_Gamma;
	__int32 m_MaxFPS;
	char _0x003C[12];
	__int32 m_GameSpeed;

	static CScreenSettings* GetInstance(void) {
		return (CScreenSettings*)0x510C94;
	}
};

struct entity
{
	short x, y, z;
	short attr1;
	unsigned char type;
	unsigned char attr2, attr3, attr4;
	unsigned char spawned;
	int lastmillis;
	char __pad[3];
};

enum WorldEntities                           // static entity types
{
	NOTUSED = 0,                // entity slot not in use in map (usually seen at deleted entities)
	LIGHT,                      // lightsource, attr1 = radius, attr2 = intensity (or attr2..4 = r-g-b)
	PLAYERSTART,                // attr1 = angle, attr2 = team
	I_CLIPS, I_AMMO, I_GRENADE, // attr1 = elevation
	I_HEALTH, I_HELMET, I_ARMOUR, I_AKIMBO,
	MAPMODEL,                   // attr1 = angle, attr2 = idx, attr3 = elevation, attr4 = texture, attr5 = pitch, attr6 = roll
	CARROT,                     // attr1 = tag, attr2 = type
	LADDER,                     // attr1 = height
	CTF_FLAG,                   // attr1 = angle, attr2 = red/blue
	SOUND,                      // attr1 = idx, attr2 = radius, attr3 = size, attr4 = volume
	CLIP,                       // attr1 = elevation, attr2 = xradius, attr3 = yradius, attr4 = height, attr6 = slope, attr7 = shape
	PLCLIP,                     // attr1 = elevation, attr2 = xradius, attr3 = yradius, attr4 = height, attr6 = slope, attr7 = shape
	DUMMYENT,                   // temporary entity without any function - will not be saved to map files, used to mark positions and for scripting
	MAXENTTYPES
};

class CWorldEntityList {
public:

	static entity* GetWorldEntity(int ID) {
		return (entity*)(*(DWORD*)0x510118 + ID * 0x14);
	}

	static int GetWorldEntityCount(void) {
		return *(int*)0x510120;
	}
};

struct flaginfo
{
	int team;
	entity* flagent;
	int actor_cn;
	CEntity* actor;
	vec pos;
	int state;
	int __pad;
};

enum FlagStatus { CTFF_INBASE = 0, CTFF_STOLEN, CTFF_DROPPED, CTFF_IDLE };

class CFlagArray {
public:
	flaginfo flags[2];

	static CFlagArray* GetInstance(void) {
		return (CFlagArray*)0x50F4A8;
	}
};

class CCamera {
public:

	static float GetFOV(void) {
		return *(float*)0x50F1C4;
	}
	static void SetFOV(float FOV) {
		*(float*)0x50F1C4 = FOV;
	}
	static glmatrixf GetViewProjection(void) {
		return *(glmatrixf*)0x501AE8;
	}
	static glmatrixf GetViewMatrix(void) {
		return *(glmatrixf*)0x501B28;
	}
	static glmatrixf GetPerspectiveMatrix(void) {
		return *(glmatrixf*)0x501BB8;
	}
};
