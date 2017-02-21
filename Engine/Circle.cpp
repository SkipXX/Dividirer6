#include "Circle.h"



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

void Circle::Update(float dt)
{
	m_pos += m_v * dt;
}
