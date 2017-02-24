#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Colors.h"

#include "SpringLink.h"

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

	virtual void Draw(Graphics& gfx) const;
	virtual void Move(Vec2 dv);
	bool IsOverlappingWith(Circle& cir) const;
};


class CircleObject : public Circle
{
private:
public:
	Vec2 m_v = Vec2(0,0);

	std::vector<SpringLink> m_links;
	bool dragging = false;
public:
	CircleObject();
	CircleObject(Vec2 pos, float radius, Color color = Colors::White);
	~CircleObject() = default;
	
	void Update(float dt);
	void Update_Links(float dt);
	void Draw(Graphics & gfx) const;

	void RemoveLinksTO(std::vector<CircleObject>& objects);

	void RemoveLinksFROM();
};