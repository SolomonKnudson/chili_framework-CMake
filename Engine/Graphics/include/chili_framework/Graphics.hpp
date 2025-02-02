/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.hpp																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <chili_framework/Colors.hpp>
#include <chili_framework/D3PipeLine.hpp>

#include <chili_framework/GraphicsException.hpp>
#include <chili_framework/GraphicsUtil.hpp>

class Graphics
{
public:
  Graphics();
  Graphics(HWND wnd);

  ~Graphics();
  Graphics(const Graphics&) = delete;

  Graphics&
  operator=(const Graphics&) = delete;

  void
  init(HWND wnd);

  void
  EndFrame();

  void
  BeginFrame();

  void
  PutPixel(int x, int y, int r, int g, int b)
  {
    PutPixel(x, y, {unsigned char(r), unsigned char(g), unsigned char(b)});
  }

  void
  PutPixel(int x, int y, Color c);

private:
  //Frame util methods
  void
  flip_buffers();

  void
  clear_sysbuffer();

  D3PipeLine m_PipeLine{};
};

#endif // !GRAPHICS_H
