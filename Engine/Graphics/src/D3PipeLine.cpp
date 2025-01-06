#include "Graphics/include/Graphics.hpp"

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
