#pragma once
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#include <math.h>

class vector {
public:
	FLOAT x;
	FLOAT y;
	FLOAT z;

	VOID subtract(vector vec) {
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
	}
};

class angles {
public:
	FLOAT yaw;
	FLOAT pitch;
	FLOAT roll;

	VOID clamp(VOID) {
		if (this->pitch < -89.0f) this->pitch = -89.0f;
		if (this->pitch > 89.0f) this->pitch = 89.0f;
		while (this->yaw < 0.0f) this->yaw += 360.0f;
		while (this->yaw > 360.0f) this->yaw -= 360.0f;
		this->roll = 0.0f;
	}
	VOID aimbot(vector vec) {
		this->pitch = ::atanf(vec.z / ::sqrtf(vec.x*vec.x + vec.y*vec.y)) * -57.295779513082f;
		this->yaw = ::atanf(vec.x / vec.y) * -57.295779513082f;
		if (vec.y < 0.0f) this->yaw += 180.0f;
		this->clamp();
	}
};

class player {
private:
	BYTE _pad0[0x34];
public:
	vector pos; //0x34
	angles rot; //0x40
private:
	BYTE _pad1[0xAC];
public:
	DWORD health; //0xF8
private:
	BYTE _pad2[0x22D];
public:
	DWORD team; //0x329
};

class playerlist {
public:
	player* remoteplayer[0x20];
};

class ACgame {
public:
	player* localplayer;
	playerlist* remoteplayerlist;
	DWORD maxcount;
	DWORD count;
};
