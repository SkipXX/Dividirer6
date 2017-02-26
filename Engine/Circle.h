#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Colors.h"

#include "SpringLink.h"

class GameObject
{
private:
public:
	Vec2 m_pos;

public:
	GameObject();
	GameObject(Vec2 pos);
	~GameObject();

	virtual void Move(Vec2 dv);
	virtual void Draw(Graphics& gfx) const = 0;
	virtual bool IsOverlappingWith(GameObject* cir) const = 0;
};


class CircleObject : public GameObject
{
private:
public:
	float m_radius;
	Color m_color;
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
	bool IsOverlappingWith(GameObject * cir) const;
};