#pragma once
#include "GameObject.h"

#define LEVELUP_WALKING_SPEED 0.1f;

#define LEVELUP_BBOX_WIDTH 16
#define LEVELUP_BBOX_HEIGHT 15


#define LEVELUP_STATE_WALKING 100
#define LEVELUP_STATE_DIE 200

#define LEVELUP_ANI_WALKING 0
#define LEVELUP_ANI_DIE 1

class CLevelup : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CLevelup();
	virtual void SetState(int state);
};