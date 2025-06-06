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
#ifndef CHILI_FRAMEWORK_GAME_H
#define CHILI_FRAMEWORK_GAME_H
#include <chili_framework/Graphics.hpp>
#include <chili_framework/Keyboard.hpp>
#include <chili_framework/MainWindow.hpp>
#include <chili_framework/Mouse.hpp>

class Game
{
public:
  Game(class MainWindow& wnd);
  Game(const Game&) = delete;

  Game&
  operator=(const Game&) = delete;

  void
  Go();

private:
  void
  ComposeFrame();

  void
  UpdateModel();

private:
  MainWindow& m_wnd;
  Graphics m_gfx{};
};
#endif // !CHILI_FRAMEWORK_GAME_H
