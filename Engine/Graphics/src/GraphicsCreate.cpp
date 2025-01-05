#include "Graphics/include/Graphics.hpp"

void
Graphics::create_device_and_swap_chain(const DXGI_SWAP_CHAIN_DESC& desc)
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
Graphics::create_view_on_back_buffer(HRESULT hr)
{
  // get handle to backbuffer
  Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer{};

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
}

void
Graphics::create_texture_for_cpu_render_target(
    const D3D11_TEXTURE2D_DESC& sysTexDesc, HRESULT hr)
{
  // create the texture
  if (GraphicsUtil::failed(hr = m_pDevice->CreateTexture2D(
                               &sysTexDesc, nullptr, &m_pSysBufferTexture)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
  }
}

void
Graphics::create_resource_view_on_texture(
    const D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc, HRESULT hr)
{
  // create the resource view on the texture
  if (GraphicsUtil::failed(
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
Graphics::create_pixel_shader_for_framebuffer(HRESULT hr)
{
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
}

void
Graphics::create_vertex_shader_for_framebuffer(HRESULT hr)
{
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
}
