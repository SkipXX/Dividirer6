#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Colors.h"

#include "SpringLink.h"

enum class GameObjectType
{
	CIRCLE,
	MAX_TYPES
};

class GameObject
{
protected:
	GameObjectType typeOfObject;
public:
	Vec2 m_pos;
	Vec2 m_v = Vec2(0,0);
	std::vector<SpringLink> m_links;

	bool dragging = false;
public:
	GameObject() = delete;
	GameObject(Vec2 pos);
	~GameObject();

	virtual void Move(Vec2 dv);
	virtual void Draw(Graphics& gfx, Vec2& Camera) const = 0;
	virtual bool IsOverlappingWith(GameObject* cir) const = 0;
	virtual bool IsInObject(Vec2& point) const = 0;
	virtual void Update(float dt) = 0;
	virtual GameObjectType GetType() const = 0;
	virtual void RemoveLinksTO(std::vector<GameObject*>& objects) = 0;
	virtual void RemoveLinksFROM() = 0;
};


class CircleObject : public GameObject
{
private:
public:
	float m_radius;
	Color m_color;

public:
	CircleObject();
	CircleObject(Vec2 pos, float radius, Color color = Colors::White);
	~CircleObject() = default;
	
	void Update(float dt);
	void Update_Links(float dt);
	void Draw(Graphics & gfx, Vec2& Offset = Vec2(0,0)) const;

	void RemoveLinksTO(std::vector<GameObject*>& objects);
	void RemoveLinksFROM();

	bool IsOverlappingWith(GameObject * cir) const;
	bool IsInObject(Vec2& point) const;
	GameObjectType GetType() const;
};