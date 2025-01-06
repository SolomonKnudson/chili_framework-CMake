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
#include <assert.h>

void
Graphics::EndFrame()
{
  HRESULT hr;

  // lock and map the adapter memory for copying over the sysbuffer
  if (GraphicsUtil::failed(hr = m_PipeLine.m_pImmediateContext->Map(
                               m_PipeLine.m_pSysBufferTexture.Get(),
                               0u,
                               D3D11_MAP_WRITE_DISCARD,
                               0u,
                               &m_PipeLine.m_mappedSysBufferTexture)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
  }
  // setup parameters for copy operation
  Color* pDst =
      reinterpret_cast<Color*>(m_PipeLine.m_mappedSysBufferTexture.pData);
  const size_t dstPitch =
      m_PipeLine.m_mappedSysBufferTexture.RowPitch / sizeof(Color);
  const size_t srcPitch = Screen::Width;
  const size_t rowBytes = srcPitch * sizeof(Color);
  // perform the copy line-by-line
  for (size_t y = 0u; y < Screen::Height; y++)
  {
    memcpy(
        &pDst[y * dstPitch], &m_PipeLine.m_pSysBuffer[y * srcPitch], rowBytes);
  }
  // release the adapter memory
  m_PipeLine.m_pImmediateContext->Unmap(m_PipeLine.m_pSysBufferTexture.Get(),
                                        0u);

  // render offscreen scene texture to back buffer
  m_PipeLine.m_pImmediateContext->IASetInputLayout(
      m_PipeLine.m_pInputLayout.Get());
  m_PipeLine.m_pImmediateContext->VSSetShader(
      m_PipeLine.m_pVertexShader.Get(), nullptr, 0u);

  m_PipeLine.m_pImmediateContext->PSSetShader(
      m_PipeLine.m_pPixelShader.Get(), nullptr, 0u);
  m_PipeLine.m_pImmediateContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  const UINT stride{sizeof(FSQVertex)};
  const UINT offset{0u};

  m_PipeLine.m_pImmediateContext->IASetVertexBuffers(
      0u, 1u, m_PipeLine.m_pVertexBuffer.GetAddressOf(), &stride, &offset);

  m_PipeLine.m_pImmediateContext->PSSetShaderResources(
      0u, 1u, m_PipeLine.m_pSysBufferTextureView.GetAddressOf());
  m_PipeLine.m_pImmediateContext->PSSetSamplers(
      0u, 1u, m_PipeLine.m_pSamplerState.GetAddressOf());
  m_PipeLine.m_pImmediateContext->Draw(6u, 0u);

  // flip back/front buffers
  if (GraphicsUtil::failed(hr = m_PipeLine.m_pSwapChain->Present(1u, 0u)))
  {
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
    {
      throw CHILI_GFX_EXCEPTION(m_PipeLine.m_pDevice->GetDeviceRemovedReason(),
                                L"Presenting back buffer [device removed]");
    }
    else
    {
      throw CHILI_GFX_EXCEPTION(hr, L"Presenting back buffer");
    }
  }
}

void
Graphics::flip_buffers(HRESULT hr)
{
  if (GraphicsUtil::failed(hr = m_PipeLine.m_pSwapChain->Present(1u, 0u)))
  {
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
    {
      throw CHILI_GFX_EXCEPTION(m_PipeLine.m_pDevice->GetDeviceRemovedReason(),
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
  clear_sysbuffer();
}

void
Graphics::clear_sysbuffer()
{
  memset(m_PipeLine.m_pSysBuffer,
         0u,
         sizeof(Color) * Screen::Height * Screen::Width);
}
