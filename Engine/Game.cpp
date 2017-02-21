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

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	m_circles.push_back(Circle(Vec2(200, 200), 15, Colors::Blue));
	m_circles.push_back(Circle(Vec2(150, 150), 10, Colors::Red));

	//m_circles.at(1).m_v = Vec2(100, 100);
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
	float dt = timer.Mark();

	inputHandling();

	if (!pause)
	{
		//Dragging
		if (!wnd.mouse.LeftIsPressed())
		{
			dragging = false;
		}

		if (wnd.mouse.LeftIsPressed())
		{
			Vec2 mousePos = Vec2(float(wnd.mouse.GetPosX()), float(wnd.mouse.GetPosY()));

			Vec2 distanceToCircle = (mousePos - m_circles.at(0).m_pos);
			if (dragging || distanceToCircle.GetLength() < m_circles.at(0).m_radius)
			{
				dragging = true;
				m_circles.at(0).m_pos = mousePos;
			}
		}

		//Feder shit
		for (auto& ii : m_circles)
		{
			if (&ii == &m_circles.at(0)) continue;
			
			Vec2 distance_v = (ii.m_pos - m_circles.at(0).m_pos);
			float distance = distance_v.GetLength();

			if (distance > Federlaenge)
			{
				//ii.m_pos -= distance_v.GetNormalized() * (distance - Federlaenge);
				ii.m_v -= distance_v.GetNormalized() * Federkonstante * 1 * (distance - Federlaenge) * dt;
			}
			if (distance < Federlaenge)
			{
				ii.m_v -= distance_v.GetNormalized() * Federkonstante * 4 * (distance - Federlaenge) * dt;
			}
		}

		//Daempfung
		for (auto& ii : m_circles)
		{
			ii.m_v *= pow(0.8f,dt);
		}

		//Gravitation
		if (m_gravitation)
		{
			for (auto& ii : m_circles)
			{
				if (&ii == &m_circles.at(0)) continue;
				ii.m_v += Vec2(0,600.0f) * dt;
			}
		}

		//Movement
		for (auto& ii : m_circles)
		{
			ii.Update(dt);
		}
	}
}

void Game::ComposeFrame()
{
	for (auto& ii : m_circles)
	{
		ii.Draw(gfx);
	}

	for (auto& ii : m_circles)
	{
		if (&ii == &m_circles.at(0)) continue;
		gfx.DrawLine(m_circles.at(0).m_pos,ii.m_pos);	
	}
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

	}
	///
}