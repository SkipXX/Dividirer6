#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Colors.h"

class Circle
{
private:

public:
	Vec2 m_pos;
	float m_radius;

	Color m_color;

public:
	Circle();
	Circle(Vec2 pos, float radius, Color color = Colors::White);
	~Circle();

	void Draw(Graphics& gfx) const;
	void Move(Vec2 dv);
};

