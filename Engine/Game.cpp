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

Game::~Game()
{
	for (auto& ii : m_objects)
	{
		delete ii;
	}
	m_objects.clear();
}



void Game::Go()
{
	gfx.BeginFrame();	

	float dt = timer.Mark();
	//time slowdown when dt too high
	if (dt > 0.02f) dt = 0.02f;
	//for testing
	//assert(dt < 0.1f);
	//if (dt > 0.1f) throw("ye");
	dt *= GameSpeed / float(Iterations);
	
	//INPUT
	inputHandling(dt);

	for (int nn = 0; nn < Iterations; nn++)
	{
		UpdateModel(dt);
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{
	
		//Dragging 1/2 /// IMPROVE THAT IT DOES NOT CHECK EVERYTHING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!wnd.mouse.LeftIsPressed())
		{
			for (auto& ii : m_objects)
			{
				if (ii->dragging)
				{
					ii->dragging = false;
					ii->m_v += (Vec2(float(wnd.mouse.GetPosX() + Camera.x - gfx.ScreenWidth / 2), float(wnd.mouse.GetPosY() + Camera.y - gfx.ScreenHeight / 2)) - LastMousePos).Normalize() * ThrowingStrenght;

				}
			}
		}

		if (!pause)
		{
			Vec2 mousePos = Vec2(float(wnd.mouse.GetPosX() + Camera.x - gfx.ScreenWidth / 2), float(wnd.mouse.GetPosY() + Camera.y - gfx.ScreenHeight / 2));
			//Draggin 2/2
			if (wnd.mouse.LeftIsPressed())
			{

				for (auto& ii : m_objects)
				{
					if (ii->dragging || ii->IsInObject(mousePos))
					{
						bool OnlyOne = true;
						for (auto& jj : m_objects)
						{
							if (&ii == &jj) continue;
							if (jj->dragging)
							{
								OnlyOne = false;
								break;
							}
						}
						if (OnlyOne)
						{
							ii->dragging = true;
							if(!m_camera && (ii->m_pos - LastMousePos).GetLengthSq() < (ii->m_pos - mousePos).GetLengthSq()) ii->m_pos += (mousePos - LastMousePos);
							//ii->m_pos += (mousePos - ii->m_pos) * dt;
							ii->m_v = Vec2(0, 0);

							thePossesed = ii;
						}

					}
				}
			}
			LastMousePos = mousePos;

			//Daempfung
			if (m_reibung)
			{
				for (auto& ii : m_objects)
				{
					ii->m_v *= pow(Daempfungsfaktor, dt);
				}
			}

			//Gravitation
			if (m_gravitation)
			{
				for (auto& ii : m_objects)
				{
					//if (&ii == &m_objects.at(0)) continue;
					ii->m_v += Vec2(0, 600.0f) * dt;
				}
			}


			//Movement
			for (auto& ii : m_objects)
			{
				ii->Update(dt);
			}


			//Ground and Wall
			DoWallCollision(dt);

			//bounce BOUNCE
			DoCircleCollision(dt);
		}



		//Upadte Camera
		if (m_camera && thePossesed) 
		{
			Camera = thePossesed->m_pos;
		}
		else
		{
			Camera = Vec2(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2);
		}

	}




void Game::ComposeFrame()
{
	int xOffset = gfx.ScreenWidth / 2 - int(Camera.x) ;
	int yOffset = gfx.ScreenHeight / 2 - int(Camera.y);
	Vec2 Offset = Vec2(float(xOffset), float(yOffset));

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
	for (auto& ii : m_objects)
	{
		ii->Draw(gfx, Offset);
	}

	//Draws all Links
	for (auto& ii : m_objects)
	{
		//if (&ii == &m_objects.at(0)) continue;
		for (auto& jj : ii->m_links)
		{
			gfx.DrawLine(ii->m_pos + Offset, *(jj.linkedPoint) + Offset);
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
	if (pause)
	{
		for (auto& ii : m_objects)
		{
			if (ii->dragging)
			{
				gfx.DrawLine(ii->m_pos + Offset, Vec2((float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY()) + Offset, Colors::Magenta);
			}
		}
	}

	//Draws Ground and Wall
	for (int y = 0; y < gfx.ScreenHeight; ++y)
	{
		for (int x = 0; x < gfx.ScreenWidth; ++x)
		{
			if(y - yOffset > gfx.ScreenHeight - 20 || x - xOffset > gfx.ScreenWidth -20) gfx.PutPixel(x, y, Colors::Gray);
		}
	}
}

void Game::DrawPossesed()
{
	if (thePossesed == nullptr) return;

	//white circle
	{
		Vec2 Offset = Vec2(float(gfx.ScreenWidth / 2 - int(Camera.x)), float(gfx.ScreenHeight / 2 - int(Camera.y)));

		CircleObject* thePossesed_cast_cir = dynamic_cast<CircleObject*>(thePossesed);
		if (thePossesed_cast_cir)
		{
			gfx.DrawCircle(int(thePossesed_cast_cir->m_pos.x) + int(Offset.x), int(thePossesed_cast_cir->m_pos.y) + int(Offset.y), int(thePossesed_cast_cir->m_radius) + 2, Colors::SoftWhite);
		}
	}

	//wierd lines
	//int ScheifAnzahl = 30;
	//
	//for (int ii = 0; ii < ScheifAnzahl; ii++)
	//{
	//	gfx.DrawLine(thePossesed->m_pos, thePossesed->m_pos + Vec2(float(rand() % 44 - 22), float(rand() % 44 - 22)), Colors::SoftWhite);
	//}
}



void Game::inputHandling(float dt)
{	
	///Mouse
	Vec2 mousePos = Vec2(float(wnd.mouse.GetPosX() + Camera.x - gfx.ScreenWidth / 2), float(wnd.mouse.GetPosY() + Camera.y - gfx.ScreenHeight / 2));

	//possesed select even in pause
	if (pause && wnd.mouse.LeftIsPressed())
	{
		for (auto& ii : m_objects)
		{
			if (ii->dragging || ii->IsInObject(mousePos))
			{
				thePossesed = ii;
			}

		}
	}

	//creating Link
	if (wnd.mouse.RightIsPressed())
	{
		for (auto& ii : m_objects)
		{
			if (ii->IsInObject(mousePos))
			{
				if (thePossesed == ii) break;
				CreateMutualLink(ii, thePossesed, Federkonstante, (ii->m_pos - thePossesed->m_pos).GetLength());

				//if (tempObjForLink == ii) break;
				//
				//if (!m_creatingLink)
				//{
				//	tempObjForLink = ii;
				//	m_creatingLink = true;
				//}
				//else
				//{
				//	CreateMutualLink(ii, tempObjForLink, Federkonstante, (ii->m_pos - tempObjForLink->m_pos).GetLength());
				//	m_creatingLink = false;
				//}

			}

		}
	}


	/// NOT BUFFERED
	//Esc to exit
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		exit(1337);
	}

	//delete the possesed
	if (wnd.kbd.KeyIsPressed(VK_DELETE) && thePossesed)
	{
		DeleteObject(thePossesed);
		thePossesed = nullptr;
	}

	//delete links of possesed
	if (wnd.kbd.KeyIsPressed(VK_SHIFT) && wnd.kbd.KeyIsPressed(VK_DELETE) && thePossesed)
	{
		thePossesed->RemoveLinksTO(m_objects);
		thePossesed->RemoveLinksFROM();
	}

	//continual the possesed movement
	if (!pause && thePossesed)
	{
		if (!wnd.kbd.KeyIsPressed(VK_SHIFT))
		{
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
		else
		{
			//if LEFT is pressed (move left)
			if (wnd.kbd.KeyIsPressed(VK_LEFT))
			{
				thePossesed->m_v.x -= moveSpeed * ShiftSpeedFaktor * dt * Iterations;
			}


			//if right is pressed (move right)
			if (wnd.kbd.KeyIsPressed(VK_RIGHT))
			{
				thePossesed->m_v.x += moveSpeed * ShiftSpeedFaktor * dt * Iterations;
			}


			//if UP is pressed (move up)
			if (wnd.kbd.KeyIsPressed(VK_UP))
			{
				thePossesed->m_v.y -= moveSpeed * ShiftSpeedFaktor * dt * Iterations;
			}


			//if DOWN is pressed (move down)
			if (wnd.kbd.KeyIsPressed(VK_DOWN))
			{
				thePossesed->m_v.y += moveSpeed * ShiftSpeedFaktor * dt * Iterations;
			}
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



	//possesed movement (jump)
	if (thePossesed)
	{
		if (!wnd.kbd.KeyIsPressed(VK_SHIFT))
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
	}



	//if 'C' is pressed (Camera ON/OFF)
	if (wnd.kbd.KeyIsPressed(0x43) && !(inputBuffer & 0x100))
	{
		m_camera = !m_camera;
		inputBuffer |= 0x100;
	}

	//if 'Einfg' is pressed (Camera ON/OFF)
	if (wnd.kbd.KeyIsPressed(VK_INSERT) && !(inputBuffer & 0x0200))
	{
		CreateCircleObject(Vec2(float(wnd.mouse.GetPosX()), float(wnd.mouse.GetPosY())), 10.0f + rand() % 11, Color(rand()%256, rand() % 256, rand() % 256));
		inputBuffer |= 0x200;
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

		// 0x100 = 'C'-Key ... Camera ON/OFF
		if ((inputBuffer & 0x100) && !wnd.kbd.KeyIsPressed(0x43))
		{
			inputBuffer &= ~0x100;
		}

		// 0x200 = 'Einfg'-Key ... Create CircleObject
		if ((inputBuffer & 0x200) && !wnd.kbd.KeyIsPressed(VK_INSERT))
		{
			inputBuffer &= ~0x200;
		}
	}
	///
}



void Game::CreateMutualLink(GameObject* C1, GameObject* C2,float c, float l)
{
	assert(C1 != C2);
	if (C1 == C2) return;

	CircleObject* C1_cast_cir = dynamic_cast<CircleObject*>(C1);
	CircleObject* C2_cast_cir = dynamic_cast<CircleObject*>(C2);
	if (C1_cast_cir && C2_cast_cir)
	{
		C1_cast_cir->m_links.push_back(SpringLink(&(C2->m_pos), c, l));
		C2_cast_cir->m_links.push_back(SpringLink(&(C1->m_pos), c, l));
	}
}


void Game::DoCircleCollision(float dt)
{

	for (auto& ii : m_objects)
	{
		//TESTCODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (ii->m_v.x > 20000.0f) pause = true; 
		////////////////////////////////////////////
		for (auto& jj : m_objects)
		{
			if (&ii == &jj) continue;

			if (ii->IsOverlappingWith(jj))
			{
				CircleObject* ii_cast_cir = dynamic_cast<CircleObject*>(ii);
				CircleObject* jj_cast_cir = dynamic_cast<CircleObject*>(jj);
				if (jj_cast_cir && ii_cast_cir)
				{
					Vec2 distance_v = (jj->m_pos - ii->m_pos);
					//ii->m_pos += (jj->m_pos - ii->m_pos) * 0.5f;
					//jj->m_pos = ii->m_pos;
					//distance_v.Normalize();
					//
					//ii->m_pos -= distance_v * (ii_cast_cir->m_radius + jj_cast_cir->m_radius) * 0.5f;
					//jj->m_pos += distance_v * (ii_cast_cir->m_radius + jj_cast_cir->m_radius) * 0.5f;
					////its not quite physicly accurate but it kinda works
					//float v = (jj->m_v.GetLength() + ii->m_v.GetLength()) / 2.0f;
					//ii->m_v -= distance_v * v * 0.05f;
					//jj->m_v += distance_v * v * 0.05f;

					distance_v.Normalize();
					ii->m_v -= distance_v * dt * 30000.0f;

				}
			}
		}
	}
}



void Game::setupObjects()
{
	for (auto& ii : m_objects)
	{
		delete ii;
	}
	m_objects.clear();

	thePossesed = nullptr;
	
	CreateCircleObject(Vec2(250, 50), 15, Colors::SoftBlue);		//0
	CreateCircleObject(Vec2(200, 100), 15, Colors::SoftRed);		//1
	CreateCircleObject(Vec2(200, 150), 15, Colors::SoftGreen);		//2
	CreateCircleObject(Vec2(250, 100), 15, Colors::SoftCyan);		//3
	CreateCircleObject(Vec2(250, 150), 15, Colors::Gray);			//4
	CreateCircleObject(Vec2(300, 100), 15, Colors::SoftMagenta);	//5
	CreateCircleObject(Vec2(400, 150), 15, Colors::SoftYellow);		//6
	CreateCircleObject(Vec2(500, 150), 15, Colors::SoftWhite);		//7
	
	
	CreateMutualLink(m_objects.at(1), m_objects.at(2), Federkonstante, Federlaenge);
	CreateMutualLink(m_objects.at(2), m_objects.at(4), Federkonstante, Federlaenge);
	CreateMutualLink(m_objects.at(3), m_objects.at(4), Federkonstante, Federlaenge);
	CreateMutualLink(m_objects.at(1), m_objects.at(3), Federkonstante, Federlaenge);
	//CreateMutualLink(m_objects.at(3), m_objects.at(5), Federkonstante, Federlaenge);
	//CreateMutualLink(m_objects.at(4), m_objects.at(6), Federkonstante, Federlaenge);
	CreateMutualLink(m_objects.at(5), m_objects.at(6), Federkonstante, Federlaenge + 30);
	
	CreateMutualLink(m_objects.at(4), m_objects.at(1), Federkonstante, Federlaenge);
	CreateMutualLink(m_objects.at(2), m_objects.at(3), Federkonstante, Federlaenge);
}



void Game::DoWallCollision(float dt)
{
	for (auto& ii : m_objects)
	{
		CircleObject* ii_cast_cir = dynamic_cast<CircleObject*>(ii);
		if (ii_cast_cir)
		{
			if (ii->m_pos.y > gfx.ScreenHeight - 20 - ii_cast_cir->m_radius)
			{
				//ii.m_pos.y = gfx.ScreenHeight - 20 - ii.m_radius;
				//ii.m_v.y = -ii.m_v.y * WallBounceFaktor;
				ii->m_v += Vec2(0.0f, -10000.0f) * dt * (ii->m_pos.y + ii_cast_cir->m_radius - (gfx.ScreenHeight - 20));

				//ii.m_v *= pow(Reibungskoeffizient, dt);
			}
			if (ii->m_pos.x > gfx.ScreenWidth - 20 - ii_cast_cir->m_radius)
			{
				//ii.m_pos.x = gfx.ScreenWidth - 20 - ii.m_radius;
				//ii.m_v.x = -ii.m_v.x * WallBounceFaktor;
				ii->m_v += Vec2(-10000.0f, 0.0f) * dt * (ii->m_pos.x + ii_cast_cir->m_radius - (gfx.ScreenWidth - 20));

				//ii.m_v *= pow(Reibungskoeffizient, dt);
			}
		}
	}
}


void Game::DeleteObject(GameObject* obj)
{
	CircleObject* obj_cast_cir = dynamic_cast<CircleObject*>(obj);
	if (true)
	{
		obj_cast_cir->RemoveLinksTO(m_objects);

		std::vector<GameObject*>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); it++)
		{
			if(&((*it)->m_pos) == &(obj->m_pos))break;
		}
		m_objects.erase(it);
	}
}


void Game::CreateCircleObject(const Vec2& pos,const float& radius,const Color& c)
{
	m_objects.push_back(static_cast<GameObject*>(new CircleObject(pos, radius, c)));
}

