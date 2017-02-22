/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

#include <cmath>
#include <cassert>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	setupObjects();
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	float dt = timer.Mark() * GameSpeed / float(Iterations);
	assert(dt < 0.1f);
	inputHandling();
	for (int nn = 0; nn < Iterations; nn++)
	{
		//Dragging 1/2
		if (!wnd.mouse.LeftIsPressed())
		{
			for (auto& ii : m_circles)
			{
				if (ii.dragging)
				{
					ii.dragging = false;
					ii.m_v += (Vec2(float(wnd.mouse.GetPosX()), float(wnd.mouse.GetPosY())) - LastMousePos) * ThrowingStrenght;

				}
			}
		}

		if (!pause)
		{
			//Draggin 2/2
			if (wnd.mouse.LeftIsPressed())
			{
				Vec2 mousePos = Vec2(float(wnd.mouse.GetPosX()), float(wnd.mouse.GetPosY()));

				for (auto& ii : m_circles)
				{
					Vec2 distanceToCircle = (mousePos - ii.m_pos);

					if (ii.dragging || distanceToCircle.GetLength() < ii.m_radius)
					{
						bool OnlyOne = true;
						for (auto& jj : m_circles)
						{
							if (&ii == &jj) continue;
							if (jj.dragging)
							{
								OnlyOne = false;
								break;
							}
						}
						if (OnlyOne)
						{
							ii.dragging = true;
							ii.m_pos = mousePos;
							ii.m_v = Vec2(0, 0);
							LastMousePos = mousePos;
						}

					}
				}
			}

			//Daempfung
			if (m_reibung)
			{
				for (auto& ii : m_circles)
				{
					ii.m_v *= pow(Daempfungsfaktor, dt);
				}
			}

			//Gravitation
			if (m_gravitation)
			{
				for (auto& ii : m_circles)
				{
					//if (&ii == &m_circles.at(0)) continue;
					ii.m_v += Vec2(0, 600.0f) * dt;
				}
			}


			//Movement
			for (auto& ii : m_circles)
			{
				ii.Update(dt);
			}


			//Ground and Wall
			for (auto& ii : m_circles)
			{
				if (ii.m_pos.y > gfx.ScreenHeight - 20 - ii.m_radius)
				{
					ii.m_pos.y = gfx.ScreenHeight - 20 - ii.m_radius;
					ii.m_v.y = 0.0f;

					//ii.m_v *= pow(Reibungskoeffizient, dt);
				}
				if (ii.m_pos.x > gfx.ScreenWidth - 20 - ii.m_radius)
				{
					ii.m_pos.x = gfx.ScreenWidth - 20 - ii.m_radius;
					ii.m_v.x = 0.0f;

					//ii.m_v *= pow(Reibungskoeffizient, dt);
				}

			//bounce BOUNCE
			DoCircleCollision(dt);
			}
		}
	}
}

void Game::ComposeFrame()
{
	for (auto& ii : m_circles)
	{
		ii.Draw(gfx);
	}

	//Draws all Links
	for (auto& ii : m_circles)
	{
		//if (&ii == &m_circles.at(0)) continue;
		for (auto& jj : ii.m_links)
		{
			gfx.DrawLine(ii.m_pos, *(jj.linkedPoint));	
		}
	}

	//Reibung und Grav toggle
	if (m_reibung)
	{	
		int x = 10;
		int y = 10;

		gfx.PutPixel(x + 0, y+ 0, Colors::White);
		gfx.PutPixel(x + 1, y+ 0, Colors::White);
		gfx.PutPixel(x + 2, y+ 0, Colors::White);
		gfx.PutPixel(x + 0, y+ 1, Colors::White);
		gfx.PutPixel(x + 2, y+ 1, Colors::White);
		gfx.PutPixel(x + 0, y+ 2, Colors::White);
		gfx.PutPixel(x + 1, y+ 2, Colors::White);
		gfx.PutPixel(x + 2, y+ 2, Colors::White);
		gfx.PutPixel(x + 0, y+ 3, Colors::White);
		gfx.PutPixel(x + 1, y+ 3, Colors::White);
		gfx.PutPixel(x + 0, y+ 4, Colors::White);
		gfx.PutPixel(x + 2, y+ 4, Colors::White);
	}
	if (m_gravitation)
	{
		int x = 15;
		int y = 10;

		gfx.PutPixel(x + 0, y + 0, Colors::White);
		gfx.PutPixel(x + 1, y + 0, Colors::White);
		gfx.PutPixel(x + 2, y + 0, Colors::White);
		gfx.PutPixel(x + 0, y + 1, Colors::White);
		gfx.PutPixel(x + 0, y + 2, Colors::White);
		gfx.PutPixel(x + 2, y + 2, Colors::White);
		gfx.PutPixel(x + 0, y + 3, Colors::White);
		gfx.PutPixel(x + 1, y + 3, Colors::White);
		gfx.PutPixel(x + 2, y + 3, Colors::White);
		gfx.PutPixel(x + 2, y + 4, Colors::White);
	}

	//Drag Vector
	for (auto& ii : m_circles)
	{
		if (ii.dragging)
		{
			gfx.DrawLine(ii.m_pos,Vec2(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()), Colors::Magenta);
		}
	}

	//Draws Ground and Wall
	gfx.DrawRect(0,gfx.ScreenHeight - 20,gfx.ScreenWidth,gfx.ScreenHeight, Colors::Gray);
	gfx.DrawRect(gfx.ScreenWidth - 20, 0, gfx.ScreenWidth, gfx.ScreenHeight, Colors::Gray);
}

void Game::inputHandling()
{
	//Esc to exit ... NOT BUFFERED
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		exit(1337);
	}

	///Options
	//pause funktion
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		inputBuffer |= 0x1;
	}

	//if G is pressed (Gravitation ON/OFF)
	if (wnd.kbd.KeyIsPressed(0x47))
	{
		inputBuffer |= 0x2;
	}

	//if R is pressed (Reibung ON/OFF)
	if (wnd.kbd.KeyIsPressed(0x52))
	{
		inputBuffer |= 0x4;
	}

	//if RETURN is pressed (reset objects)
	if (wnd.kbd.KeyIsPressed(VK_RETURN))
	{
		inputBuffer |= 0x8;
	}

	if (inputBuffer)
	{
		// 0x1 = 'SPACE'-Key ... pause funktion
		if ((inputBuffer & 0x1) && !wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			pause = !pause;
			inputBuffer &= ~0x1;
		}

		// 0x2 = 'g'-Key ... Gravitation ON/OFF
		if ((inputBuffer & 0x2) && !wnd.kbd.KeyIsPressed(0x47))
		{
			m_gravitation = !m_gravitation;
			inputBuffer &= ~0x2;
		}

		// 0x4 = 'r'-Key ... Gravitation ON/OFF
		if ((inputBuffer & 0x4) && !wnd.kbd.KeyIsPressed(0x52))
		{
			m_reibung = !m_reibung;
			inputBuffer &= ~0x4;
		}

		// 0x8 = 'RETURN'-Key ... reset objects
		if ((inputBuffer & 0x8) && !wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			setupObjects();
			inputBuffer &= ~0x8;
		}
	}
	///
}

void Game::CreateMutualLink(CircleObject* C1, CircleObject* C2,float c, float l)
{
	assert(C1 != C2);
	if (C1 == C2) return;
	C1->m_links.push_back(SpringLink(&(C2->m_pos), c, l));
	C2->m_links.push_back(SpringLink(&(C1->m_pos), c, l));
}

void Game::DoCircleCollision(float dt)
{
	for (auto& ii : m_circles)
	{
		for (auto& jj : m_circles)
		{
			if (&ii == &jj) continue;

			if (ii.IsOverlappingWith(jj))
			{
				Vec2 distance_v = (jj.m_pos - ii.m_pos).Normalize();
				ii.m_pos = jj.m_pos;
				ii.m_pos -= distance_v * (ii.m_radius + jj.m_radius);
				//its not quite physicly accurate but it kinda works
				float v = (jj.m_v.GetLength() + ii.m_v.GetLength()) / 2.0f;
				ii.m_v -= distance_v * v * (1.0f / float(Iterations));
				jj.m_v += distance_v * v * (1.0f / float(Iterations));
			}
		}
	}
}

void Game::setupObjects()
{
	m_circles.clear();

	m_circles.push_back(CircleObject(Vec2(150, 50), 15, Colors::Blue));		//0
	m_circles.push_back(CircleObject(Vec2(100, 100), 15, Colors::Red));		//1
	m_circles.push_back(CircleObject(Vec2(100, 150), 15, Colors::Green));	//2
	m_circles.push_back(CircleObject(Vec2(150, 100), 15, Colors::Cyan));	//3
	m_circles.push_back(CircleObject(Vec2(150, 150), 15, Colors::Gray));	//4
	//m_circles.push_back(CircleObject(Vec2(200, 100), 15, Colors::Magenta));	//5
	//m_circles.push_back(CircleObject(Vec2(200, 150), 15, Colors::Yellow));	//6


	CreateMutualLink(&m_circles.at(1), &m_circles.at(2), Federkonstante, Federlaenge);
	//CreateMutualLink(&m_circles.at(2), &m_circles.at(4), Federkonstante, Federlaenge);
	//CreateMutualLink(&m_circles.at(3), &m_circles.at(4), Federkonstante, Federlaenge);
	//CreateMutualLink(&m_circles.at(1), &m_circles.at(3), Federkonstante, Federlaenge);
	//CreateMutualLink(&m_circles.at(1), &m_circles.at(2), Federkonstante, Federlaenge);
	//CreateMutualLink(&m_circles.at(1), &m_circles.at(2), Federkonstante, Federlaenge);
	//CreateMutualLink(&m_circles.at(1), &m_circles.at(2), Federkonstante, Federlaenge);
}