#pragma once
#include "Vec2.h"
#include <vector>

class CircleObject;

class SpringLink
{
public:
	float springConstant = 1.0f;
	float springLength = 1.0f;

	Vec2* linkedPoint;

public:
	SpringLink();
	SpringLink(Vec2* p, float springConstant, float springLength);
	~SpringLink();
};

