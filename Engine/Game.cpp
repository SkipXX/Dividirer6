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
	double dt = timer.Mark();
	
	//for testing
	//assert(dt < 0.1f);
	//if (dt > 0.1f) throw("ye");
	
	dt *= GameSpeed / double(Iterations);

	//INPUT
	inputHandling(dt);

	//possesed select even in pause
	if (pause && wnd.mouse.LeftIsPressed())
	{
		Vec2 mousePos = Vec2(double(wnd.mouse.GetPosX()), double(wnd.mouse.GetPosY()));
		for (auto& ii : m_circles)
		{
			Vec2 distanceToCircle = (mousePos - ii.m_pos);

			if (ii.dragging || distanceToCircle.GetLength() < ii.m_radius)
			{
				thePossesed = &ii;
			}

		}
	}
	

	for (int nn = 0; nn < Iterations; nn++)
	{
		//Dragging 1/2 /// IMPROVE THAT IT DOES NOT CHECK EVERYTHING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!wnd.mouse.LeftIsPressed())
		{
			for (auto& ii : m_circles)
			{
				if (ii.dragging)
				{
					ii.dragging = false;
					ii.m_v += (Vec2(double(wnd.mouse.GetPosX()), double(wnd.mouse.GetPosY())) - LastMousePos) * ThrowingStrenght;

				}
			}
		}

		if (!pause)
		{
			//Draggin 2/2
			if (wnd.mouse.LeftIsPressed())
			{
				Vec2 mousePos = Vec2(double(wnd.mouse.GetPosX()), double(wnd.mouse.GetPosY()));

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

							thePossesed = &ii;
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
			DoWallCollision(dt);

			//bounce BOUNCE
			DoCircleCollision(dt);
		}
	}
}



void Game::ComposeFrame()
{
	//BG
	for (int xx = 0; xx < gfx.ScreenWidth; xx++)
	{
		for (int yy = 0; yy < gfx.ScreenHeight; yy++)
		{
			gfx.PutPixel(xx,yy,Color(30,30,30));
		}
	}

	//thePossesed
	if(thePossesed) DrawPossesed();

	//Circles
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
			gfx.DrawLine(ii.m_pos,Vec2((double)wnd.mouse.GetPosX(), (double)wnd.mouse.GetPosY()), Colors::Magenta);
		}
	}

	//Draws Ground and Wall
	gfx.DrawRect(0,gfx.ScreenHeight - 20,gfx.ScreenWidth,gfx.ScreenHeight, Colors::Gray);
	gfx.DrawRect(gfx.ScreenWidth - 20, 0, gfx.ScreenWidth, gfx.ScreenHeight, Colors::Gray);
}

void Game::DrawPossesed()
{
	if (thePossesed == nullptr) return;

	//white circle
	gfx.DrawCircle((int)thePossesed->m_pos.x, (int)thePossesed->m_pos.y, int(thePossesed->m_radius) + 2, Colors::SoftWhite);


	//wierd lines
	//int ScheifAnzahl = 30;
	//
	//for (int ii = 0; ii < ScheifAnzahl; ii++)
	//{
	//	gfx.DrawLine(thePossesed->m_pos, thePossesed->m_pos + Vec2(double(rand() % 44 - 22), double(rand() % 44 - 22)), Colors::SoftWhite);
	//}
}



void Game::inputHandling(double dt)
{	
	/// NOT BUFFERED
	//Esc to exit
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		exit(1337);
	}

	//delete the possesed
	if (wnd.kbd.KeyIsPressed(VK_DELETE) && thePossesed)
	{
		//thePossesed->RemoveLinksTO(m_circles);
		//thePossesed->RemoveLinksFROM();

		DeleteObject(thePossesed);

		thePossesed = nullptr;
	}


	//Movement
	if (!pause && thePossesed)
	{
		//continual movement
		//if LEFT is pressed (move left)
		if (wnd.kbd.KeyIsPressed(VK_LEFT))
		{
			thePossesed->m_v.x -= moveSpeed * dt * Iterations;
		}


		//if right is pressed (move right)
		if (wnd.kbd.KeyIsPressed(VK_RIGHT))
		{
			thePossesed->m_v.x += moveSpeed * dt * Iterations;
		}


		//if UP is pressed (move up)
		if (wnd.kbd.KeyIsPressed(VK_UP))
		{
			thePossesed->m_v.y -= moveSpeed * dt * Iterations;
		}


		//if DOWN is pressed (move down)
		if (wnd.kbd.KeyIsPressed(VK_DOWN))
		{
			thePossesed->m_v.y += moveSpeed * dt * Iterations;
		} 
	
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
	if (wnd.kbd.KeyIsPressed(VK_RETURN) && !(inputBuffer & 0x8))
	{
		setupObjects();
		inputBuffer |= 0x8;
	}

	if (thePossesed)
	{
		//if LEFT is pressed (move left)
		if (wnd.kbd.KeyIsPressed(VK_LEFT) && !(inputBuffer & 0x10))
		{
			thePossesed->m_v.x -= moveSpeed;
			inputBuffer |= 0x10;
		}


		//if right is pressed (move right)
		if (wnd.kbd.KeyIsPressed(VK_RIGHT) && !(inputBuffer & 0x20))
		{
			thePossesed->m_v.x += moveSpeed;
			inputBuffer |= 0x20;
		}


		//if UP is pressed (move up)
		if (wnd.kbd.KeyIsPressed(VK_UP) && !(inputBuffer & 0x40))
		{
			thePossesed->m_v.y -= moveSpeed;
			inputBuffer |= 0x40;
		}


		//if DOWN is pressed (move down)
		if (wnd.kbd.KeyIsPressed(VK_DOWN) && !(inputBuffer & 0x80))
		{
			thePossesed->m_v.y += moveSpeed;
			inputBuffer |= 0x80;
		}
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
			inputBuffer &= ~0x8;
		}

		// 0x10 = 'LEFT'-Key ... move left
		if ((inputBuffer & 0x10) && !wnd.kbd.KeyIsPressed(VK_LEFT))
		{
			inputBuffer &= ~0x10;
		}

		// 0x20 = 'RIGHT'-Key ... move right
		if ((inputBuffer & 0x20) && !wnd.kbd.KeyIsPressed(VK_RIGHT))
		{
			inputBuffer &= ~0x20;
		}

		// 0x40 = 'UP'-Key ... move UP
		if ((inputBuffer & 0x40) && !wnd.kbd.KeyIsPressed(VK_UP))
		{
			inputBuffer &= ~0x40;
		}

		// 0x80 = 'DOWN'-Key ... move down
		if ((inputBuffer & 0x80) && !wnd.kbd.KeyIsPressed(VK_DOWN))
		{
			inputBuffer &= ~0x80;
		}
	}
	///
}



void Game::CreateMutualLink(CircleObject* C1, CircleObject* C2,double c, double l)
{
	assert(C1 != C2);
	if (C1 == C2) return;
	C1->m_links.push_back(SpringLink(&(C2->m_pos), c, l));
	C2->m_links.push_back(SpringLink(&(C1->m_pos), c, l));
}

void Game::DoCircleCollision(double dt)
{

	for (auto& ii : m_circles)
	{
		//TESTCODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (ii.m_v.x > 50000.0f) pause = true; 
		////////////////////////////////////////////
		for (auto& jj : m_circles)
		{
			if (&ii == &jj) continue;

			if (ii.IsOverlappingWith(jj))
			{
				Vec2 distance_v = (jj.m_pos - ii.m_pos);
				ii.m_pos += (jj.m_pos - ii.m_pos) * 0.5f;
				jj.m_pos = ii.m_pos;
				distance_v.Normalize();

				ii.m_pos -= distance_v * (ii.m_radius + jj.m_radius) * 0.5f;
				jj.m_pos += distance_v * (ii.m_radius + jj.m_radius) * 0.5f;
				//its not quite physicly accurate but it kinda works
				double v = (jj.m_v.GetLength() + ii.m_v.GetLength()) / 2.0f;
				ii.m_v -= distance_v * v * 0.05f;
				jj.m_v += distance_v * v * 0.05f;
			}
		}
	}
}

void Game::setupObjects()
{
	m_circles.clear();

	m_circles.push_back(CircleObject(Vec2(250, 50), 15, Colors::SoftBlue));		//0
	m_circles.push_back(CircleObject(Vec2(200, 100), 15, Colors::SoftRed));		//1
	m_circles.push_back(CircleObject(Vec2(200, 150), 15, Colors::SoftGreen));	//2
	m_circles.push_back(CircleObject(Vec2(250, 100), 15, Colors::SoftCyan));	//3
	m_circles.push_back(CircleObject(Vec2(250, 150), 15, Colors::Gray));		//4
	m_circles.push_back(CircleObject(Vec2(300, 100), 15, Colors::SoftMagenta));	//5
	m_circles.push_back(CircleObject(Vec2(400, 150), 15, Colors::SoftYellow));	//6
	m_circles.push_back(CircleObject(Vec2(500, 150), 15, Colors::SoftWhite));	//7


	CreateMutualLink(&m_circles.at(1), &m_circles.at(2), Federkonstante, Federlaenge);
	CreateMutualLink(&m_circles.at(2), &m_circles.at(4), Federkonstante, Federlaenge);
	CreateMutualLink(&m_circles.at(3), &m_circles.at(4), Federkonstante, Federlaenge);
	CreateMutualLink(&m_circles.at(1), &m_circles.at(3), Federkonstante, Federlaenge);
		//CreateMutualLink(&m_circles.at(3), &m_circles.at(5), Federkonstante, Federlaenge);
		//CreateMutualLink(&m_circles.at(4), &m_circles.at(6), Federkonstante, Federlaenge);
	CreateMutualLink(&m_circles.at(5), &m_circles.at(6), Federkonstante, Federlaenge + 30);

	CreateMutualLink(&m_circles.at(4), &m_circles.at(1), Federkonstante, Federlaenge);
	CreateMutualLink(&m_circles.at(2), &m_circles.at(3), Federkonstante, Federlaenge);
}

void Game::DoWallCollision(double dt)
{
	for (auto& ii : m_circles)
	{
		if (ii.m_pos.y > gfx.ScreenHeight - 20 - ii.m_radius)
		{
			//ii.m_pos.y = gfx.ScreenHeight - 20 - ii.m_radius;
			//ii.m_v.y = -ii.m_v.y * WallBounceFaktor;
			ii.m_v += Vec2(0.0f,-10000.0f) * dt * (ii.m_pos.y + ii.m_radius - (gfx.ScreenHeight - 20));

			//ii.m_v *= pow(Reibungskoeffizient, dt);
		}
		if (ii.m_pos.x > gfx.ScreenWidth - 20 - ii.m_radius)
		{
			//ii.m_pos.x = gfx.ScreenWidth - 20 - ii.m_radius;
			//ii.m_v.x = -ii.m_v.x * WallBounceFaktor;
			ii.m_v += Vec2(-10000.0f, 0.0f) * dt * (ii.m_pos.x + ii.m_radius - (gfx.ScreenWidth - 20));

			//ii.m_v *= pow(Reibungskoeffizient, dt);
		}
	}
}

void Game::DeleteObject(CircleObject* obj)
{
	obj->RemoveLinksTO(m_circles);

	///does not work because of Links that point at objects
	//std::vector<CircleObject>::iterator it;
	//for (it = m_circles.begin(); it != m_circles.end(); it++)
	//{
	//	if(&(it->m_pos) == &(obj->m_pos))break;
	//}
	//m_circles.erase(it);

	*obj = CircleObject();
}
