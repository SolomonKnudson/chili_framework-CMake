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
#include "Game.hpp"

Game::Game(MainWindow& wnd)
  : m_wnd{wnd}
  , m_gfx{wnd.key()}
{
}

void
Game::Go()
{
  m_gfx.BeginFrame();
  UpdateModel();

  ComposeFrame();
  m_gfx.EndFrame();
}

void
Game::UpdateModel()
{
}

void
Game::ComposeFrame()
{
  for (int i{}, x{}, y{}; i < 500; ++i, ++x, ++y)
  {
    m_gfx.PutPixel(x, y, Colors::Red);
  }

  for (int i{}, x{500}; i < 300; ++i, ++x)
  {
    m_gfx.PutPixel(x, 500, Colors::Magenta);
  }

  for (int i{}, y{500}; i < 500; ++i, --y)
  {
    m_gfx.PutPixel(500, y, Colors::Magenta);
  }
}
