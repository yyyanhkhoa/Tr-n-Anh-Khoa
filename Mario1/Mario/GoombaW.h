#pragma once
#include "GameObject.h"

#define GOOMBAW_WALKING_SPEED 0.1f;

#define GOOMBAW_BBOX_WIDTH 16
#define GOOMBAW_BBOX_HEIGHT 15


#define GOOMBAW_STATE_WALKING 100
#define GOOMBAW_STATE_DIE 200

#define GOOMBAW_ANI_WALKING 0
#define GOOMBAW_ANI_DIE 1

class CGoombaW : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	CGoombaW();
	virtual void SetState(int state);	
};