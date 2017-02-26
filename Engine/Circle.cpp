#include "Circle.h"

#define MY_INNENKONSTANTE 1.0f
#define MY_AUSSENKONSTANTE 1.0f

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
}

CircleObject::CircleObject(Vec2 pos, float radius, Color color)
	:
	GameObject(pos),
	m_radius(radius),
	m_color(color)
{
}


void CircleObject::Update(float dt)
{
	Update_Links(dt);
	m_pos += m_v * dt;
}

void CircleObject::Update_Links(float dt)
{
	for (auto& ii : m_links)
	{
		Vec2 distance_v = (*(ii.linkedPoint) - m_pos);
		float distance = distance_v.GetLength();
		
		//ii.m_pos -= distance_v.GetNormalized() * (distance - Federlaenge);
		m_v += distance_v.GetNormalized() * ii.springConstant * MY_INNENKONSTANTE * (distance - ii.springLength) * dt;
		
	}
}

void CircleObject::Draw(Graphics& gfx) const
{
	if (m_pos.y - 1 > gfx.ScreenHeight - 20 - m_radius)
	{
		gfx.DrawEllipse(int(m_pos.x), int(m_pos.y), int(m_radius), Colors::Black);
		gfx.DrawEllipse(int(m_pos.x), int(m_pos.y), int(m_radius - 2), m_color);


		gfx.DrawEllipse(int(m_pos.x + m_radius / 4), int(m_pos.y - m_radius / 4), int(m_radius / 3),
			Color(m_color.GetR() + 60 < 255 ? m_color.GetR() + 60 : 255,
				m_color.GetG() + 60 < 255 ? m_color.GetG() + 60 : 255,
				m_color.GetB() + 60 < 255 ? m_color.GetB() + 60 : 255));
	}
	else
	{
		gfx.DrawCircle(int(m_pos.x), int(m_pos.y), int(m_radius), Colors::Black);
		gfx.DrawCircle(int(m_pos.x), int(m_pos.y), int(m_radius - 2), m_color);


		gfx.DrawCircle(int(m_pos.x + m_radius / 4), int(m_pos.y - m_radius / 4), int(m_radius / 3),
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

bool CircleObject::IsOverlappingWith(GameObject * cir) const
{
	CircleObject* cast_cir = dynamic_cast<CircleObject*>(cir);
	if (cast_cir)
	{
		float distance{ (m_pos - cast_cir->m_pos).GetLength() };

		if (distance < m_radius + cast_cir->m_radius) return true;
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
