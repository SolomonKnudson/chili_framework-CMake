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
#include "Graphics/include/Graphics.h"
#include "DXErr/include/DXErr.h"
#include "MainWindow/include/MainWindow.h" //Must be included here

#include <array>
#include <assert.h>
#include <string>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FrameBuffers/include/FramebufferPS.shh"
#include "FrameBuffers/include/FramebufferVS.shh"
} // namespace FramebufferShaders

using Microsoft::WRL::ComPtr;

Graphics::Graphics()
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

Graphics::Graphics(HWNDKey& key)
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
  assert(key.m_hWnd != nullptr);
  //////////////////////////////////////////////////////
  // create device and swap chain/get render target view
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 1;
  sd.BufferDesc.Width = Graphics::ScreenWidth;
  sd.BufferDesc.Height = Graphics::ScreenHeight;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 1;
  sd.BufferDesc.RefreshRate.Denominator = 60;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = key.m_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  HRESULT hr{};
  UINT createFlags{0u};

#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
  createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

  // create device and front/back buffers
  if (GraphicsUtil::failed(
          hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                             D3D_DRIVER_TYPE_HARDWARE,
                                             nullptr,
                                             createFlags,
                                             nullptr,
                                             0,
                                             D3D11_SDK_VERSION,
                                             &sd,
                                             &m_pSwapChain,
                                             &m_pDevice,
                                             nullptr,
                                             &m_pImmediateContext)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating device and swap chain");
  }

  // get handle to backbuffer
  ComPtr<ID3D11Resource> pBackBuffer{};
  if (GraphicsUtil::failed(
          hr = m_pSwapChain->GetBuffer(
              0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Getting back buffer");
  }

  // create a view on backbuffer that we can render to
  if (GraphicsUtil::failed(
          hr = m_pDevice->CreateRenderTargetView(
              pBackBuffer.Get(), nullptr, &m_pRenderTargetView)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
  }

  // set backbuffer as the render target using created view
  m_pImmediateContext->OMSetRenderTargets(
      1, m_pRenderTargetView.GetAddressOf(), nullptr);

  // set viewport dimensions
  D3D11_VIEWPORT vp;
  vp.Width = float(Graphics::ScreenWidth);
  vp.Height = float(Graphics::ScreenHeight);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0.0f;
  vp.TopLeftY = 0.0f;
  m_pImmediateContext->RSSetViewports(1, &vp);

  ///////////////////////////////////////
  // create texture for cpu render target
  D3D11_TEXTURE2D_DESC sysTexDesc;
  sysTexDesc.Width = Graphics::ScreenWidth;
  sysTexDesc.Height = Graphics::ScreenHeight;
  sysTexDesc.MipLevels = 1;
  sysTexDesc.ArraySize = 1;
  sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sysTexDesc.SampleDesc.Count = 1;
  sysTexDesc.SampleDesc.Quality = 0;
  sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
  sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  sysTexDesc.MiscFlags = 0;
  // create the texture
  if (GraphicsUtil::failed(hr = m_pDevice->CreateTexture2D(
                               &sysTexDesc, nullptr, &m_pSysBufferTexture)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = sysTexDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  // create the resource view on the texture
  if (GraphicsUtil::failed(
          hr = m_pDevice->CreateShaderResourceView(
              m_pSysBufferTexture.Get(), &srvDesc, &m_pSysBufferTextureView)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating view on sysBuffer texture");
  }

  ////////////////////////////////////////////////
  // create pixel shader for framebuffer
  // Ignore the intellisense error "namespace has no member"
  if (GraphicsUtil::failed(
          hr = m_pDevice->CreatePixelShader(
              FramebufferShaders::FramebufferPSBytecode,
              sizeof(FramebufferShaders::FramebufferPSBytecode),
              nullptr,
              &m_pPixelShader)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating pixel shader");
  }

  /////////////////////////////////////////////////
  // create vertex shader for framebuffer
  // Ignore the intellisense error "namespace has no member"
  if (GraphicsUtil::failed(
          hr = m_pDevice->CreateVertexShader(
              FramebufferShaders::FramebufferVSBytecode,
              sizeof(FramebufferShaders::FramebufferVSBytecode),
              nullptr,
              &m_pVertexShader)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex shader");
  }

  //////////////////////////////////////////////////////////////
  // create and fill vertex buffer with quad for rendering frame
  const FSQVertex vertices[] = {
      {-1.0f, 1.0f, 0.5f, 0.0f, 0.0f},
      {1.0f, 1.0f, 0.5f, 1.0f, 0.0f},
      {1.0f, -1.0f, 0.5f, 1.0f, 1.0f},
      {-1.0f, 1.0f, 0.5f, 0.0f, 0.0f},
      {1.0f, -1.0f, 0.5f, 1.0f, 1.0f},
      {-1.0f, -1.0f, 0.5f, 0.0f, 1.0f},
  };
  D3D11_BUFFER_DESC bd = {};
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(FSQVertex) * 6;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0u;
  D3D11_SUBRESOURCE_DATA initData = {};
  initData.pSysMem = vertices;
  if (GraphicsUtil::failed(
          hr = m_pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex buffer");
  }

  //////////////////////////////////////////
  // create input layout for fullscreen quad
  const D3D11_INPUT_ELEMENT_DESC ied[] = {{"POSITION",
                                           0,
                                           DXGI_FORMAT_R32G32B32_FLOAT,
                                           0,
                                           0,
                                           D3D11_INPUT_PER_VERTEX_DATA,
                                           0},
                                          {"TEXCOORD",
                                           0,
                                           DXGI_FORMAT_R32G32_FLOAT,
                                           0,
                                           12,
                                           D3D11_INPUT_PER_VERTEX_DATA,
                                           0}};

  // Ignore the intellisense error "namespace has no member"
  if (FAILED(hr = m_pDevice->CreateInputLayout(
                 ied,
                 2,
                 FramebufferShaders::FramebufferVSBytecode,
                 sizeof(FramebufferShaders::FramebufferVSBytecode),
                 &m_pInputLayout)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating input layout");
  }

  ////////////////////////////////////////////////////
  // Create sampler state for fullscreen textured quad
  D3D11_SAMPLER_DESC sampDesc = {};
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  if (GraphicsUtil::failed(
          hr = m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating sampler state");
  }

  // allocate memory for sysbuffer (16-byte aligned for faster access)
  m_pSysBuffer = reinterpret_cast<Color*>(_aligned_malloc(
      sizeof(Color) * Graphics::ScreenWidth * Graphics::ScreenHeight, 16u));
}

Graphics::~Graphics()
{
  // free sysbuffer memory (aligned free)
  if (m_pSysBuffer)
  {
    _aligned_free(m_pSysBuffer);
    m_pSysBuffer = nullptr;
  }

  // clear the state of the device context before destruction
  if (m_pImmediateContext)
    m_pImmediateContext->ClearState();
}

void
Graphics::EndFrame()
{
  HRESULT hr;

  // lock and map the adapter memory for copying over the sysbuffer
  if (FAILED(hr = m_pImmediateContext->Map(m_pSysBufferTexture.Get(),
                                           0u,
                                           D3D11_MAP_WRITE_DISCARD,
                                           0u,
                                           &m_mappedSysBufferTexture)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
  }
  // setup parameters for copy operation
  Color* pDst = reinterpret_cast<Color*>(m_mappedSysBufferTexture.pData);
  const size_t dstPitch = m_mappedSysBufferTexture.RowPitch / sizeof(Color);
  const size_t srcPitch = Graphics::ScreenWidth;
  const size_t rowBytes = srcPitch * sizeof(Color);
  // perform the copy line-by-line
  for (size_t y = 0u; y < Graphics::ScreenHeight; y++)
  {
    memcpy(&pDst[y * dstPitch], &m_pSysBuffer[y * srcPitch], rowBytes);
  }
  // release the adapter memory
  m_pImmediateContext->Unmap(m_pSysBufferTexture.Get(), 0u);

  // render offscreen scene texture to back buffer
  m_pImmediateContext->IASetInputLayout(m_pInputLayout.Get());
  m_pImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);

  m_pImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
  m_pImmediateContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  const UINT stride{sizeof(FSQVertex)};
  const UINT offset{0u};

  m_pImmediateContext->IASetVertexBuffers(
      0u, 1u, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

  m_pImmediateContext->PSSetShaderResources(
      0u, 1u, m_pSysBufferTextureView.GetAddressOf());
  m_pImmediateContext->PSSetSamplers(0u, 1u, m_pSamplerState.GetAddressOf());
  m_pImmediateContext->Draw(6u, 0u);

  // flip back/front buffers
  if (GraphicsUtil::failed(hr = m_pSwapChain->Present(1u, 0u)))
  {
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
    {
      throw CHILI_GFX_EXCEPTION(m_pDevice->GetDeviceRemovedReason(),
                                L"Presenting back buffer [device removed]");
    }
    else
    {
      throw CHILI_GFX_EXCEPTION(hr, L"Presenting back buffer");
    }
  }
}

void
Graphics::BeginFrame()
{
  // clear the sysbuffer
  memset(m_pSysBuffer,
         0u,
         sizeof(Color) * Graphics::ScreenHeight * Graphics::ScreenWidth);
}

void
Graphics::PutPixel(int x, int y, Color c)
{
  assert(x >= 0);
  assert(x < int(Graphics::ScreenWidth));

  assert(y >= 0);
  assert(y < int(Graphics::ScreenHeight));

  m_pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception(HRESULT hr,
                               const std::wstring& note,
                               const wchar_t* file,
                               unsigned int line)
  : ChiliException(file, line, note)
  , hr(hr)
{
}

std::wstring
Graphics::Exception::GetFullMessage() const
{
  const std::wstring empty = L"";
  const std::wstring errorName = GetErrorName();

  const std::wstring errorDesc = GetErrorDescription();
  const std::wstring& note = GetNote();

  const std::wstring location = GetLocation();

  return (!errorName.empty() ? std::wstring(L"Error: ") + errorName + L"\n"
                             : empty) +
         (!errorDesc.empty()
              ? std::wstring(L"Description: ") + errorDesc + L"\n"
              : empty) +
         (!note.empty() ? std::wstring(L"Note: ") + note + L"\n" : empty) +
         (!location.empty() ? std::wstring(L"Location: ") + location : empty);
}

std::wstring
Graphics::Exception::GetErrorName() const
{
  return DXGetErrorString(hr);
}

std::wstring
Graphics::Exception::GetErrorDescription() const
{
  std::array<wchar_t, 512> wideDescription;
  DXGetErrorDescription(hr, wideDescription.data(), wideDescription.size());
  return wideDescription.data();
}

std::wstring
Graphics::Exception::GetExceptionType() const
{
  return L"Chili Graphics Exception";
}
