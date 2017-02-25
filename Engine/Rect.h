#pragma once

#include "Vec2.h"
#include "Graphics.h"

class Rect
{
public:
	double m_left, m_right, m_top, m_bottom;

public:
	Rect();
	Rect(double left, double right, double top, double bottom);
	Rect(Vec2 left_top, Vec2 right_bottom);
	Rect(Vec2 top_left, double width, double height);
	~Rect();

	static Rect fromMiddle(Vec2 middle, double halfWidth, double halfHeight);

	bool isOverlappingWith(const Rect& rekt) const;

	virtual void move(const Vec2& dv);
	void move(double dx, double dy);
	virtual void moveTo(const Vec2& left_top);
	virtual void Draw(Graphics& gfx) const;
	virtual void Draw(Graphics& gfx, Color c) const;
};

