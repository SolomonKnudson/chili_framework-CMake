#include "Graphics/include/Graphics.hpp"

namespace GraphicsUtil
{
} // namespace GraphicsUtil

void
Graphics::create_device_and_swap_chain(const HWND key)
{
  DXGI_SWAP_CHAIN_DESC sd{
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

  };

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
                                             &sd,
                                             &m_pSwapChain,
                                             &m_pDevice,
                                             nullptr,
                                             &m_pImmediateContext)))
  {
    throw CHILI_GFX_EXCEPTION(hr, L"Creating device and swap chain");
  }
}
