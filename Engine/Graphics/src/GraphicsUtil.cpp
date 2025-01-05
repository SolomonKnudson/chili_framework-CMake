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
  create_texture_for_cpu_render_target(sysTexDesc, hr);

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

  srvDesc.Format = sysTexDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;

  create_resource_view_on_texture(srvDesc, hr);

  create_pixel_shader_for_framebuffer(hr);

  create_vertex_shader_for_framebuffer(hr);

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
Graphics::set_viewport_dimensions(const D3D11_VIEWPORT& vp)
{
  m_pImmediateContext->RSSetViewports(1, &vp);
}
