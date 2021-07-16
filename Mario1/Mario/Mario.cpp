#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "GoombaW.h"
#include "Goomba.h"
#include "Portal.h"
#include "Koopas.h"
#include "PlayScence.h"
#include "levelup.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;


	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	// set mario die when he too low
	if (die == true) 
	{
		if (GetTickCount() - untouchable_start > 1000)
		{
			
		}		
		SetState(AFTER_DIE);
	}
	if (y > 180) 
	{		
		SetState(MARIO_STATE_DIE);			
		die = true;		
	}
	
	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// if no collision occurred, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;
				
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
				
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;
		

		// Collision logic with other objects		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{			
			LPCOLLISIONEVENT e = coEventsResult[i];

			y1 = y + (e->dy);
			
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{						
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}				
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			
			if (dynamic_cast<CGoombaW*>(e->obj)) // if e->obj is Goomba 
			{
				CGoombaW* goombaW = dynamic_cast<CGoombaW*>(e->obj);

				// jump on top >> kill Goombaw and deflect a bit 
				if (e->ny < 0)
				{
					if (goombaW->GetState() != GOOMBAW_STATE_DIE)
					{
						goombaW->SetState(GOOMBAW_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->ny > 0)
				{
					if (untouchable == 0)
					{
						if (goombaW->GetState() != GOOMBAW_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goombaW->GetState() != GOOMBAW_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if GoombaW

			if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
								
				if (Koopas->GetState() == KOOPAS_STATE_DIE)
				{					
					if (e->nx > 0)
					{				
						StartUntouchable();
						Koopas->SetState(KOOPAS_DIE_WALKING);
						Koopas->dx = trunc(sqrt(dx * dx));
						
					}
					if (e->nx < 0) 
					{
						StartUntouchable();
						Koopas->SetState(KOOPAS_DIE_WALKING);
						Koopas->dx = -trunc(sqrt(dx * dx));
					}
				}

				// jump on top and kill
				if (e->ny < 0)
				{
					if (Koopas->GetState() != KOOPAS_STATE_DIE)
					{
						Koopas->SetState(KOOPAS_STATE_DIE);						
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (Koopas->GetState() != KOOPAS_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if koopas

			if (dynamic_cast<CLevelup*>(e->obj)) // if e->obj is levelup
			{
				CLevelup* Levelup = dynamic_cast<CLevelup*>(e->obj);
				Levelup->SetState(LEVELUP_STATE_DIE);
				if (level == MARIO_LEVEL_SMALL) 
				{
					vy = -0.3f;
					level = MARIO_LEVEL_BIG;
				}			
			} // if levelup
			

			if (dynamic_cast<CPortal*>(e->obj)) // mario meet portal
			{				
				CPortal* p = dynamic_cast<CPortal*>(e->obj);	
					
				if (p->GetSceneId() == 1  )
				{					
					if (MessageBoxA(NULL, (LPCSTR)"You win.\n Do you want to play again ?", (LPSTR)"Game over", MB_YESNO) == IDNO)
					{
						exit(0);
					}
				}								
				CGame::GetInstance()->SwitchScene(p->GetSceneId());				
			}			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
	if (level == MARIO_LEVEL_BIG)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (vx > 0) 
			ani = MARIO_ANI_BIG_WALKING_RIGHT; 
		else ani = MARIO_ANI_BIG_WALKING_LEFT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED_Y;					
		break; 
	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;		
		break;			
	case AFTER_DIE :
		y = 250;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}


void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}



