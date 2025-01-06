/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	D3PipeLineInit.cpp																	  *
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

void
D3PipeLine::init(const HWND wnd)
{
  // create device and swap chain/get render target view
  create_device_and_swap_chain({
      {Screen::Width,                  //BufferDesc: Width
       Screen::Height,                 //BufferDesc: Height
       {1, 60},                        //BufferDesc: RefreshRate
       DXGI_FORMAT_B8G8R8A8_UNORM,     //BufferDesc: Format
       {},                             //BufferDesc: ScanlineOrdering
       {}},                            //BufferDesc: Scaling
      {1, 0},                          //SampleDesc: Count, Quality}
      DXGI_USAGE_RENDER_TARGET_OUTPUT, //BufferUsage
      1,                               //BufferCount
      wnd,                             //OutputWindow
      true,                            //Windowed
      {},                              //SwapEffect
      {}                               //Flags
  });

  // create a view on backbuffer that we can render to
  create_view_on_back_buffer();

  set_viewport_dimensions({0.0f,                               //TopLeftX
                           0.0f,                               //TopLeftY
                           static_cast<float>(Screen::Width),  //Width
                           static_cast<float>(Screen::Height), //Height
                           0.0f,                               //MinDepth
                           1.0f});                             //MaxDepth

  // create texture for cpu render target
  D3D11_TEXTURE2D_DESC sysTexDesc{Screen::Width,              //Width
                                  Screen::Height,             //Height
                                  1,                          //MipLevels
                                  1,                          //ArraySize
                                  DXGI_FORMAT_B8G8R8A8_UNORM, //Format
                                  {1, 0}, //SampleDesc: {Count, Quality}
                                  D3D11_USAGE_DYNAMIC,        //Usage
                                  D3D11_BIND_SHADER_RESOURCE, //BindFlags
                                  D3D11_CPU_ACCESS_WRITE,     //CPUAccessFlags
                                  0};                         //MiscFlags

  create_texture_for_cpu_render_target(sysTexDesc);

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

  srvDesc.Format = sysTexDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;

  create_resource_view_on_texture(srvDesc);

  create_pixel_shader_for_framebuffer();

  create_vertex_shader_for_framebuffer();

  create_and_fill_vertex_buffer();

  create_input_layout_for_fullscreen_quad();

  create_sampler_state_for_fullscreen_textured_quad();

  allocate_memory_for_sysbuffer(); //(16-byte aligned for faster access)
}

void
D3PipeLine::set_viewport_dimensions(const D3D11_VIEWPORT& vp) const
{
  m_pImmediateContext->RSSetViewports(1, &vp);
}

void
D3PipeLine::allocate_memory_for_sysbuffer()
{
  m_pSysBuffer = reinterpret_cast<Color*>(
      _aligned_malloc(sizeof(Color) * Screen::Width * Screen::Height, 16u));
}
