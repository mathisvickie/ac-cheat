#include "geom.h"
#include "Reversed.hpp"

class CRageBot {
public:
	static void DoRageBot(bool);

	static bool CanHit(vec point) {

		auto local = CGame::GetInstance()->local;
		traceresult_s tr;

		TraceLine(local->GetHeadPosition(), point, local, false, &tr, false);

		return !tr.collided;
	}

	static vec LookAt(vec at) {

		vec angles;
		vec v(CGame::GetInstance()->local->GetHeadPosition().sub(at));
		
		angles.x = ::atanf(v.x / v.y) * -57.2957795f;
		angles.y = ::atanf(v.z / ::sqrtf(v.x*v.x + v.y*v.y)) * -57.2957795f;
		angles.z = 0.f;

		if (v.y < 0.0f)
			angles.x += 180.0f;

		while (angles.x < 0.f)
			angles.x += 360.f;

		while (angles.x >= 360.f)
			angles.x -= 360.f;

		if (angles.y > 90.f)
			angles.y = 90.f;

		if (angles.y < -90.f)
			angles.y = -90.0;

		return angles;
	}
};

void CRageBot::DoRageBot(bool bShootHead) {

	auto ac = CGame::GetInstance();
	auto local = ac->local;

	if (!local || !ac->list)
		return;

	local->SetShootState(false);

	for (int i = 0; i < ac->GetPlayerCount(); i++) {

		auto ent = ac->list->ent[i];

		if (!ent || ent->GetHealth() <= 0)
			continue;

		if (ac->IsTeamMode() && ent->GetTeam() == local->GetTeam())
			continue;

		vec head = ent->GetHeadPosition();
		head.z -= 1.f;

		if (!CRageBot::CanHit(head))
			continue;

		local->SetViewAngles(CRageBot::LookAt(head));
		local->SetShootState(true);
	}

	return;
}
