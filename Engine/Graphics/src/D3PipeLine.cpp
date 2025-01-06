/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	D3PipeLine.cpp																		  *
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
#include <assert.h>

D3PipeLine::D3PipeLine()
  : m_pSwapChain{}
  , m_pDevice{}

  , m_pImmediateContext{}
  , m_pRenderTargetView{}

  , m_pSysBufferTexture{}
  , m_pSysBufferTextureView{}

  , m_pPixelShader{}
  , m_pVertexShader{}

  , m_pVertexBuffer{}
  , m_pInputLayout{}

  , m_pSamplerState{}
  , m_mappedSysBufferTexture{}

  , m_pSysBuffer{}
{
}

D3PipeLine::D3PipeLine(HWND wnd)
  : m_pSwapChain{}
  , m_pDevice{}

  , m_pImmediateContext{}
  , m_pRenderTargetView{}

  , m_pSysBufferTexture{}
  , m_pSysBufferTextureView{}

  , m_pPixelShader{}
  , m_pVertexShader{}

  , m_pVertexBuffer{}
  , m_pInputLayout{}

  , m_pSamplerState{}
  , m_mappedSysBufferTexture{}

  , m_pSysBuffer{}
{
  assert(wnd);
  init(wnd);
}
