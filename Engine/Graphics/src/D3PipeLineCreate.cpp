/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	D3PipeLineCreate.cpp																		  *
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
D3PipeLine::create_device_and_swap_chain(const DXGI_SWAP_CHAIN_DESC& desc)
{
  UINT createFlags{0u};

#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
  createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

  // create device and front/back buffers
  if (HRESULT hr{}; GraphicsUtil::failed(
          hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                             D3D_DRIVER_TYPE_HARDWARE,
                                             nullptr,
                                             createFlags,
                                             nullptr,
                                             0,
                                             D3D11_SDK_VERSION,
                                             &desc,
                                             &m_pSwapChain,
                                             &m_pDevice,
                                             nullptr,
                                             &m_pImmediateContext)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating device and swap chain");
  }
}

void
D3PipeLine::create_view_on_back_buffer()
{
  // get handle to backbuffer
  Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer{};

  if (HRESULT hr{}; GraphicsUtil::failed(
          hr = m_pSwapChain->GetBuffer(
              0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Getting back buffer");
  }

  // create a view on backbuffer that we can render to
  if (HRESULT hr{}; GraphicsUtil::failed(
          hr = m_pDevice->CreateRenderTargetView(
              pBackBuffer.Get(), nullptr, &m_pRenderTargetView)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
  }

  // set backbuffer as the render target using created view
  m_pImmediateContext->OMSetRenderTargets(
      1, m_pRenderTargetView.GetAddressOf(), nullptr);
}

void
D3PipeLine::create_texture_for_cpu_render_target(
    const D3D11_TEXTURE2D_DESC& sysTexDesc)
{
  // create the texture
  if (HRESULT hr{};
      GraphicsUtil::failed(hr = m_pDevice->CreateTexture2D(
                               &sysTexDesc, nullptr, &m_pSysBufferTexture)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
  }
}

void
D3PipeLine::create_resource_view_on_texture(
    const D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc)
{
  // create the resource view on the texture
  if (HRESULT hr{}; GraphicsUtil::failed(
          hr = m_pDevice->CreateShaderResourceView(
              m_pSysBufferTexture.Get(), &srvDesc, &m_pSysBufferTextureView)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating view on sysBuffer texture");
  }
}

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FrameBuffers/include/FramebufferPS.shh"
#include "FrameBuffers/include/FramebufferVS.shh"
} // namespace FramebufferShaders

void
D3PipeLine::create_pixel_shader_for_framebuffer()
{
  // create pixel shader for framebuffer
  // Ignore the intellisense error "namespace has no member"
  if (HRESULT hr{}; GraphicsUtil::failed(
          hr = m_pDevice->CreatePixelShader(
              FramebufferShaders::FramebufferPSBytecode,
              sizeof(FramebufferShaders::FramebufferPSBytecode),
              nullptr,
              &m_pPixelShader)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating pixel shader");
  }
}

void
D3PipeLine::create_vertex_shader_for_framebuffer()
{
  // Ignore the intellisense error "namespace has no member"
  if (HRESULT hr{}; GraphicsUtil::failed(
          hr = m_pDevice->CreateVertexShader(
              FramebufferShaders::FramebufferVSBytecode,
              sizeof(FramebufferShaders::FramebufferVSBytecode),
              nullptr,
              &m_pVertexShader)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex shader");
  }
}
