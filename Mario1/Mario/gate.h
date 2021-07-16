#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  0
#define BRICK_BBOX_HEIGHT 0

class CGate : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};