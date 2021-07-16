#include "Goomba.h"
#include "Game.h"
#include "PlayScence.h"
#include "Mario.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	
	if (state == GOOMBA_STATE_DIE)
		bottom = y +GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CGameObject::Update(dt);
		
	x += dx;
	y += dy;	
	if (wX1 == 0 && wX2 == 0) 
	{
		if (vx < 0 && x < vtrX - 35)
		{
			x = vtrX - 35; vx = -vx;
		}

		if (vx > 0 && x > vtrX + 245)
		{
			x = vtrX + 245; vx = -vx;
		}
	}
	else 
	{
		if (vx < 0 && x < wX1)
		{
			x = wX1; vx = -vx;
		}

		if (vx > 0 && x > wX2)
		{
			x = wX2; vx = -vx;
		}
	}
	
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x,y);

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT -GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}
