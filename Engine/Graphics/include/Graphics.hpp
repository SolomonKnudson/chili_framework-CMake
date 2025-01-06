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
#include "Graphics/include/D3PipeLine.hpp"
#include "Graphics/include/GraphicsUtil.hpp"

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

  Graphics();
  Graphics(HWND wnd);

  ~Graphics();
  Graphics(const Graphics&) = delete;

  void
  init(HWND wnd);

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
  flip_buffers(HRESULT hr);

  void
  clear_sysbuffer();

  D3PipeLine m_PipeLine{};
};

#endif // !GRAPHICS_H
