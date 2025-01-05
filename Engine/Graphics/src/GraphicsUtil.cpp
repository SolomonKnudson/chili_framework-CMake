#include "Graphics/include/Graphics.hpp"

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FrameBuffers/include/FramebufferPS.shh"
#include "FrameBuffers/include/FramebufferVS.shh"
} // namespace FramebufferShaders

namespace GraphicsUtil
{
} // namespace GraphicsUtil

void
Graphics::init(const HWND key)
{
  // create device and swap chain/get render target view
  create_device_and_swap_chain({
      {Graphics::ScreenWidth,          //BufferDesc: Width
       Graphics::ScreenHeight,         //BufferDesc: Height
       {1, 60},                        //BufferDesc: RefreshRate
       DXGI_FORMAT_B8G8R8A8_UNORM,     //BufferDesc: Format
       {},                             //BufferDesc: ScanlineOrdering
       {}},                            //BufferDesc: Scaling
      {1, 0},                          //SampleDesc: Count, Quality}
      DXGI_USAGE_RENDER_TARGET_OUTPUT, //BufferUsage
      1,                               //BufferCount
      key,                             //OutputWindow
      true,                            //Windowed
      {},                              //SwapEffect
      {}                               //Flags

  });
  HRESULT hr{};

  // create a view on backbuffer that we can render to
  create_view_on_back_buffer(hr);

  // set viewport dimensions
  set_viewport_dimensions({0.0f,                                      //TopLeftX
                           0.0f,                                      //TopLeftY
                           static_cast<float>(Graphics::ScreenWidth), //Width
                           static_cast<float>(Graphics::ScreenHeight), //Height
                           0.0f,   //MinDepth
                           1.0f}); //MaxDepth

  // create texture for cpu render target
  D3D11_TEXTURE2D_DESC sysTexDesc{Graphics::ScreenWidth,      //Width
                                  Graphics::ScreenHeight,     //Height
                                  1,                          //MipLevels
                                  1,                          //ArraySize
                                  DXGI_FORMAT_B8G8R8A8_UNORM, //Format
                                  {1, 0}, //SampleDesc: {Count, Quality}
                                  D3D11_USAGE_DYNAMIC,        //Usage
                                  D3D11_BIND_SHADER_RESOURCE, //BindFlags
                                  D3D11_CPU_ACCESS_WRITE,     //CPUAccessFlags
                                  0};                         //MiscFlags

  // create the texture
  if (GraphicsUtil::failed(hr = m_pDevice->CreateTexture2D(
                               &sysTexDesc, nullptr, &m_pSysBufferTexture)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

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
  D3D11_BUFFER_DESC bd{(sizeof(FSQVertex) * 6),  //ByteWidth
                       D3D11_USAGE_DEFAULT,      //Usage
                       D3D11_BIND_VERTEX_BUFFER, //BindFlags
                       0u,                       //CPUAccessFlags
                       {},                       //MiscFlags
                       {}};                      //StructureByteStride

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

void
Graphics::create_device_and_swap_chain(const DXGI_SWAP_CHAIN_DESC& context)
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
                                             &context,
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
Graphics::set_viewport_dimensions(const D3D11_VIEWPORT& vp)
{
  m_pImmediateContext->RSSetViewports(1, &vp);
}
