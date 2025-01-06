/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
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
#ifndef D3PIPE_LINE_H
#define D3PIPE_LINE_H

#include "Colors/include/Colors.hpp"
#include "Graphics/include/GraphicsUtil.hpp"

#include <cassert>
#include <d3d11.h>
#include <wrl.h>

enum Screen
{
  Width = 800,
  Height = 600
};

// vertex format for the framebuffer
// fullscreen textured quad
struct FSQVertex
{
  //position
  float x{};
  float y{};
  float z{};

  //texcoords
  float u{};
  float v{};
};

struct D3PipeLine
{
  D3PipeLine();

  D3PipeLine(HWND wnd);

  void
  init(HWND wnd);

  void
  create_device_and_swap_chain(const DXGI_SWAP_CHAIN_DESC& desc);

  void
  create_view_on_back_buffer(HRESULT hr);

  void
  set_viewport_dimensions(const D3D11_VIEWPORT& vp) const;

  void
  create_texture_for_cpu_render_target(const D3D11_TEXTURE2D_DESC& sysTexDesc,
                                       HRESULT hr);

  void
  create_resource_view_on_texture(const D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc,
                                  HRESULT hr);

  void
  create_pixel_shader_for_framebuffer(HRESULT hr);

  void
  create_vertex_shader_for_framebuffer(HRESULT hr);

  D3D11_MAPPED_SUBRESOURCE m_mappedSysBufferTexture{};
  Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain{};

  Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice{};
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pImmediateContext{};

  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView{};
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pSysBufferTexture{};

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSysBufferTextureView{};
  Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader{};

  Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader{};
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer{};

  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout{};
  Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState{};

  Color* m_pSysBuffer{};
};
#endif // !D3PIPE_LINE_H
