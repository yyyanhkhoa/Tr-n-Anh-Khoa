#include "levelup.h"
CLevelup::CLevelup()
{
	SetState(LEVELUP_STATE_WALKING);
}

void CLevelup::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + LEVELUP_BBOX_WIDTH;

	if (state == LEVELUP_STATE_DIE)
		bottom = y;
	else
		bottom = y + LEVELUP_BBOX_HEIGHT;
}

void CLevelup::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);	
	vy = 0;
	vx = 0;
		
}

void CLevelup::Render()
{
	int ani = LEVELUP_ANI_WALKING;
	if (state == LEVELUP_STATE_DIE) {
		ani = LEVELUP_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CLevelup::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEVELUP_STATE_DIE:
		y += 10000;
		vx = 0;
		vy = 0;
		break;
	case LEVELUP_STATE_WALKING:
		vx = -LEVELUP_WALKING_SPEED;
	}
}
