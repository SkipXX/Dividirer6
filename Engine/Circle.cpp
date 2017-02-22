#include "Circle.h"

////////Circle/////////////

Circle::Circle()
	:
	m_pos(0.0f,0.0f),
	m_radius(0.0f),
	m_color(Colors::White)
{
}

Circle::Circle(Vec2 pos, float radius, Color color)
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
	float distance{(m_pos - cir.m_pos).GetLength()};

	if (distance < m_radius + cir.m_radius) return true;
	else return false;
}





///////CircleObject//////////

CircleObject::CircleObject()
	:
	Circle()
{
}

CircleObject::CircleObject(Vec2 pos, float radius, Color color)
	:
	Circle(pos, radius, color)
{
}


void CircleObject::Update(float dt)
{
	Update_Links(dt);
	m_pos += m_v * dt;

	//Ground
	if (m_pos.y > 580.0f - m_radius)
	{
		m_pos.y = 580.0f - m_radius;
		m_v.y = 0.0f;
	}
}

void CircleObject::Update_Links(float dt)
{
	for (auto& ii : m_links)
	{
		Vec2 distance_v = (*(ii.linkedPoint) - m_pos);
		float distance = distance_v.GetLength();

		if (distance > ii.springLength)
		{
			//ii.m_pos -= distance_v.GetNormalized() * (distance - Federlaenge);
			m_v += distance_v.GetNormalized() * ii.springConstant * 1 * (distance - ii.springLength) * dt;
		}
		if (distance < ii.springLength)
		{
			m_v += distance_v.GetNormalized() * ii.springConstant * 4 * (distance - ii.springLength) * dt;
		}
	}
}
