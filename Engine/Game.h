/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include "FrameTimer.h"
#include <vector>

#include "MyObjects.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	~Game();
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(GameObject* ii,float dt);
	/********************************/
	/*  User Functions              */
	void inputHandling(float dt);
	void CreateMutualLink(GameObject * C1, GameObject * C2, float c, float l);
	void DoCircleCollision(GameObject* ii, float dt);
	void setupObjects();
	void DoWallCollision(GameObject* ii, float dt);
	void DrawPossesed();

	void DeleteObject(GameObject * obj);
	void CreateCircleObject(const Vec2 & pos, const float & radius, const Color & c);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	std::vector<GameObject*> m_objects;

	Vec2 LastMousePos;
	Vec2 shootTO;
	bool m_shooting = false;
	Vec2 Camera = Vec2(gfx.ScreenWidth/2,gfx.ScreenHeight/2);
	GameObject* tempObjForLink;

	float Federlaenge = 50.0f;
	float Federkonstante = 100.0f;
	float Daempfungsfaktor = 0.5f;
	float ThrowingStrenght = 25.0f;
	float Reibungskoeffizient = 0.1f;
	float GameSpeed = 1.0f;
	float ShiftSpeedFaktor = 3.0f;


	int Iterations = 600;

	float moveSpeed = 500.0f;
	GameObject* thePossesed = nullptr;

	FrameTimer timer;

	uint16_t inputBuffer = 0x0;
	bool pause = false;
	bool m_gravitation = true;
	bool m_reibung = true;
	bool m_camera = false;
	bool m_creatingLink = false; //not used
	bool m_walls_lt = true;
	/********************************/
};