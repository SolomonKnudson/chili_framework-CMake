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

Graphics::Graphics(HWND wnd)
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
  , m_hr(hr)
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
  return DXGetErrorString(m_hr);
}

std::wstring
Graphics::Exception::GetErrorDescription() const
{
  std::array<wchar_t, 512> wideDescription{};
  DXGetErrorDescription(m_hr, wideDescription.data(), wideDescription.size());

  return wideDescription.data();
}

std::wstring
Graphics::Exception::GetExceptionType() const
{
  return L"Chili Graphics Exception";
}
