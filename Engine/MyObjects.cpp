#include "MyObjects.h"

#define MY_DICHTE 0.0014f
#define MY_PI 3.1415f


////////Circle/////////////

GameObject::GameObject(Vec2 pos)
	:
	m_pos(pos)
{

}

GameObject::~GameObject()
{
}

void GameObject::Move(Vec2 dv)
{
	m_pos += dv;
}

///////CircleObject//////////

CircleObject::CircleObject()
	:
	GameObject(Vec2(0.0f,0.0f))
{
	typeOfObject = GameObjectType::CIRCLE;
}

CircleObject::CircleObject(Vec2 pos, float radius, Color color)
	:
	GameObject(pos),
	m_radius(radius),
	m_color(color)
{
	typeOfObject = GameObjectType::CIRCLE;
	m_invMass = 1.0f / (MY_DICHTE * m_radius * m_radius * MY_PI);
}


void CircleObject::Update(float dt)
{
	Update_Links(dt);
	m_v += m_force * m_invMass;
	m_force = Vec2(0, 0);
	m_pos += m_v * dt;
}

void CircleObject::Update_Links(float dt)
{
	for (auto& ii : m_links)
	{
		Vec2 distance_v = (*(ii.linkedPoint) - m_pos);
		float distance = distance_v.GetLength();
		
		//ii.m_pos -= distance_v.GetNormalized() * (distance - Federlaenge);
		m_force += distance_v.GetNormalized() * ii.springConstant * (distance - ii.springLength) * dt;
		
	}
}

void CircleObject::Draw(Graphics& gfx, Vec2& Offset) const
{
	int xOffset = int(Offset.x);
	int yOffset = int(Offset.y);

	if (m_pos.y - 1 > gfx.ScreenHeight - 20 - m_radius)
	{
		gfx.DrawEllipse(int(m_pos.x) + xOffset, int(m_pos.y) + yOffset, int(m_radius), Colors::Black);
		gfx.DrawEllipse(int(m_pos.x) + xOffset, int(m_pos.y) + yOffset, int(m_radius - 2), m_color);


		gfx.DrawEllipse(int(m_pos.x + m_radius / 4) + xOffset, int(m_pos.y - m_radius / 4) + yOffset, int(m_radius / 3),
			Color(m_color.GetR() + 60 < 255 ? m_color.GetR() + 60 : 255,
				m_color.GetG() + 60 < 255 ? m_color.GetG() + 60 : 255,
				m_color.GetB() + 60 < 255 ? m_color.GetB() + 60 : 255));
	}
	else
	{
		gfx.DrawCircle(int(m_pos.x) + xOffset, int(m_pos.y) + yOffset, int(m_radius), Colors::Black);
		gfx.DrawCircle(int(m_pos.x) + xOffset, int(m_pos.y) + yOffset, int(m_radius - 2), m_color);


		gfx.DrawCircle(int(m_pos.x + m_radius / 4) + xOffset, int(m_pos.y - m_radius / 4) + yOffset, int(m_radius / 3),
			Color(m_color.GetR() + 60 < 255 ? m_color.GetR() + 60 : 255,
				m_color.GetG() + 60 < 255 ? m_color.GetG() + 60 : 255,
				m_color.GetB() + 60 < 255 ? m_color.GetB() + 60 : 255));
	}
}

void CircleObject::RemoveLinksTO(std::vector<GameObject*>& objects)
{
	for (auto& ii : objects)
	{
		if (ii == this) continue;

		bool found = false;

		for(auto& jj : ii->m_links)
		{
			if (jj.linkedPoint == &m_pos)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			std::vector<SpringLink> temp;

			for (auto& jj : ii->m_links)
			{
				if (!(jj.linkedPoint == &m_pos))
				{
					temp.push_back(jj);
				}
			}

			ii->m_links = temp;
		}
	}
}

void CircleObject::RemoveLinksFROM()
{
	m_links.clear();
}

bool CircleObject::IsOverlappingWith(GameObject * obj) const
{
	if (obj->GetType() == GameObjectType::CIRCLE)
	{
		CircleObject* cast_cir = static_cast<CircleObject*>(obj);
		float distanceSq{ (m_pos - cast_cir->m_pos).GetLengthSq() };

		if (distanceSq < (m_radius + cast_cir->m_radius) * (m_radius + cast_cir->m_radius)) return true;
		else return false;
	}
	else
	{
		throw("unexpected type of object after casting [CircleObject::IsOverlappingWith()]");
		return false;
	}
}

bool CircleObject::IsInObject(Vec2 & point) const
{
	return (point - m_pos).GetLengthSq() < m_radius*m_radius;
}

GameObjectType CircleObject::GetType() const
{
	return typeOfObject;
}
