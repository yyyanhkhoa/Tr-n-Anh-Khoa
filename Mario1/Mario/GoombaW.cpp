#include "GoombaW.h"
CGoombaW::CGoombaW()
{
	SetState(GOOMBAW_STATE_WALKING);
}

void CGoombaW::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBAW_BBOX_WIDTH;

	if (state == GOOMBAW_STATE_DIE)
		bottom = y ;
	else 	
		bottom = y + GOOMBAW_BBOX_HEIGHT;
}

void CGoombaW::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < vtrX-35) 
	{
		x = vtrX-35; vx = -vx;
	}

	if (vx > 0 && x > vtrX + 245) {
		x = vtrX + 245; vx = -vx;
	}
}

void CGoombaW::Render()
{
	int ani = GOOMBAW_ANI_WALKING;
	if (state == GOOMBAW_STATE_DIE) {
		ani = GOOMBAW_ANI_DIE;		
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoombaW ::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBAW_STATE_DIE:						
			y += 10000;
			vx = 0;
			vy = 0;
			break;
		case GOOMBAW_STATE_WALKING: 
			vx = -GOOMBAW_WALKING_SPEED;
	}
}
