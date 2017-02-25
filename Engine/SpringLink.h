#pragma once
#include "Vec2.h"
#include <vector>

class CircleObject;

class SpringLink
{
public:
	double springConstant = 1.0f;
	double springLength = 1.0f;

	Vec2* linkedPoint;

public:
	SpringLink();
	SpringLink(Vec2* p, double springConstant, double springLength);
	~SpringLink();
};

