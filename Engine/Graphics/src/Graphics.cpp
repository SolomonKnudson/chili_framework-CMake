/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
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
#include "Graphics/include/Graphics.hpp"
#include "DXErr/include/DXErr.hpp"

#include <array>
#include <assert.h>
#include <string>

Graphics::Graphics()
  : m_PipeLine{}
{
}

Graphics::Graphics(HWND wnd)
  : m_PipeLine{wnd}
{
}

Graphics::~Graphics()
{
  // free sysbuffer memory (aligned free)
  if (m_PipeLine.m_pSysBuffer)
  {
    _aligned_free(m_PipeLine.m_pSysBuffer);
    m_PipeLine.m_pSysBuffer = nullptr;
  }

  // clear the state of the device context before destruction
  if (m_PipeLine.m_pImmediateContext)
    m_PipeLine.m_pImmediateContext->ClearState();
}

void
Graphics::PutPixel(int x, int y, Color c)
{
  assert(x >= 0);
  assert(y >= 0);

  assert(x < Screen::Width);
  assert(y < Screen::Height);

  m_PipeLine.m_pSysBuffer[Screen::Width * y + x] = c;
}

void
Graphics::init(const HWND wnd)
{
  m_PipeLine.init(wnd);
}
