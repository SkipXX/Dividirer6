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

#include "Circle.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	void inputHandling(double dt);
	void CreateMutualLink(CircleObject * C1, CircleObject * C2, double c, double l);
	void DoCircleCollision(double dt);
	void setupObjects();
	void DoWallCollision();
	void DrawPossesed();

	void DeleteObject(CircleObject * obj);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	std::vector<CircleObject> m_circles;

	Vec2 LastMousePos;

	double Federlaenge = 50.0f;
	double Federkonstante = 300.0f;
	double Daempfungsfaktor = 0.5f;
	double ThrowingStrenght = 20.0f;
	double Reibungskoeffizient = 0.1f;
	double WallBounceFaktor = 1.0f;
	double GameSpeed = 1.0f;
	int Iterations = 1000;

	double moveSpeed = 500.0f;
	CircleObject* thePossesed = nullptr;

	FrameTimer timer;

	uint8_t inputBuffer = 0x0;
	bool pause = false;
	bool m_gravitation = true;
	bool m_reibung = true;
	/********************************/
};