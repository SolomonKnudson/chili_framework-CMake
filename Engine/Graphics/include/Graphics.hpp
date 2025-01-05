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
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "CHiliException/include/ChiliException.hpp"
#include "Colors/include/Colors.hpp"
#include "Graphics/include/GraphicsUtil.hpp"

#include <d3d11.h>
#include <wrl.h>

class Graphics
{
public:
  class Exception : public ChiliException
  {
  public:
    Exception(HRESULT hr,
              const std::wstring& note,
              const wchar_t* file,
              unsigned int line);

    std::wstring
    GetErrorName() const;

    std::wstring
    GetErrorDescription() const;

    virtual std::wstring
    GetFullMessage() const override;

    virtual std::wstring
    GetExceptionType() const override;

  private:
    HRESULT m_hr{};
  };

private:
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

public:
  Graphics();
  Graphics(HWND key);
  Graphics(const Graphics&) = delete;
  ~Graphics();

  Graphics&
  operator=(const Graphics&) = delete;

  void
  EndFrame();

  void
  BeginFrame();

  void
  PutPixel(int x, int y, int r, int g, int b)
  {
    PutPixel(x, y, {unsigned char(r), unsigned char(g), unsigned char(b)});
  }

  void
  PutPixel(int x, int y, Color c);

private:
  void
  init(HWND Key);

  void
  Graphics::create_device_and_swap_chain(const DXGI_SWAP_CHAIN_DESC& desc);

  void
  create_view_on_back_buffer(HRESULT hr);

  void
  Graphics::set_viewport_dimensions(const D3D11_VIEWPORT& vp);

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

public:
  static constexpr int ScreenWidth{800};
  static constexpr int ScreenHeight{600};
};

#endif // !GRAPHICS_H
