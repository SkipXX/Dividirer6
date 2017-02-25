#include "Circle.h"

#define MY_INNENKONSTANTE 1.0f
#define MY_AUSSENKONSTANTE 1.0f

////////Circle/////////////

Circle::Circle()
	:
	m_pos(0.0f,0.0f),
	m_radius(0.0f),
	m_color(Colors::White)
{
}

Circle::Circle(Vec2 pos, double radius, Color color)
	:
	m_pos(pos),
	m_radius(radius),
	m_color(color)
{

}

Circle::~Circle()
{
}

void Circle::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(int(m_pos.x), int(m_pos.y), int(m_radius), m_color);
}

void Circle::Move(Vec2 dv)
{
	m_pos += dv;
}

bool Circle::IsOverlappingWith(Circle & cir) const
{
	double distance{(m_pos - cir.m_pos).GetLength()};

	if (distance < m_radius + cir.m_radius) return true;
	else return false;
}





///////CircleObject//////////

CircleObject::CircleObject()
	:
	Circle()
{
}

CircleObject::CircleObject(Vec2 pos, double radius, Color color)
	:
	Circle(pos, radius, color)
{
}


void CircleObject::Update(double dt)
{
	Update_Links(dt);
	m_pos += m_v * dt;
}

void CircleObject::Update_Links(double dt)
{
	for (auto& ii : m_links)
	{
		Vec2 distance_v = (*(ii.linkedPoint) - m_pos);
		double distance = distance_v.GetLength();
		
		//ii.m_pos -= distance_v.GetNormalized() * (distance - Federlaenge);
		m_v += distance_v.GetNormalized() * ii.springConstant * MY_INNENKONSTANTE * (distance - ii.springLength) * dt;
		
	}
}

void CircleObject::Draw(Graphics& gfx) const
{
	gfx.DrawCircle(int(m_pos.x), int(m_pos.y), int(m_radius), Colors::Black);
	gfx.DrawCircle(int(m_pos.x), int(m_pos.y), int(m_radius - 2), m_color);


	gfx.DrawCircle(int(m_pos.x + m_radius/4), int(m_pos.y - m_radius/4), int(m_radius / 3),
		Color(	m_color.GetR() + 60 < 255 ? m_color.GetR() + 60 : 255,
				m_color.GetG() + 60 < 255 ? m_color.GetG() + 60 : 255,
				m_color.GetB() + 60 < 255 ? m_color.GetB() + 60 : 255));
}

void CircleObject::RemoveLinksTO(std::vector<CircleObject>& objects)
{
	for (auto& ii : objects)
	{
		if (&ii == this) continue;

		bool found = false;

		for(auto& jj : ii.m_links)
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

			for (auto& jj : ii.m_links)
			{
				if (!(jj.linkedPoint == &m_pos))
				{
					temp.push_back(jj);
				}
			}

			ii.m_links = temp;
		}
	}
}

void CircleObject::RemoveLinksFROM()
{
	m_links.clear();
}
