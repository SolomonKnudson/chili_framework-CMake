/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Colors.h																			  *
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
#ifndef CHILI_FRAMEWORK_COLOR_H
#define CHILI_FRAMEWORK_COLOR_H
class Color
{
public:
  constexpr Color()
    : m_dword{}
  {
  }

  constexpr Color(const Color& col)
    : m_dword(col.m_dword)
  {
  }

  constexpr Color(unsigned int dw)
    : m_dword(dw)
  {
  }

  constexpr Color(unsigned char x,
                  unsigned char r,
                  unsigned char g,
                  unsigned char b)
    : m_dword((x << 24u) | (r << 16u) | (g << 8u) | b)
  {
  }

  constexpr Color(unsigned char r, unsigned char g, unsigned char b)
    : m_dword((r << 16u) | (g << 8u) | b)
  {
  }

  constexpr Color(Color col, unsigned char x)
    : Color((x << 24u) | col.m_dword)
  {
  }

  Color&
  operator=(Color color)
  {
    m_dword = color.m_dword;
    return *this;
  }

  constexpr unsigned char
  GetX() const
  {
    return m_dword >> 24u;
  }

  constexpr unsigned char
  GetA() const
  {
    return GetX();
  }

  constexpr unsigned char
  GetR() const
  {
    return (m_dword >> 16u) & 0xFFu;
  }

  constexpr unsigned char
  GetG() const
  {
    return (m_dword >> 8u) & 0xFFu;
  }

  constexpr unsigned char
  GetB() const
  {
    return m_dword & 0xFFu;
  }

  void
  SetX(unsigned char x)
  {
    m_dword = (m_dword & 0xFFFFFFu) | (x << 24u);
  }

  void
  SetA(unsigned char a)
  {
    SetX(a);
  }

  void
  SetR(unsigned char r)
  {
    m_dword = (m_dword & 0xFF00FFFFu) | (r << 16u);
  }

  void

  SetG(unsigned char g)
  {
    m_dword = (m_dword & 0xFFFF00FFu) | (g << 8u);
  }

  void
  SetB(unsigned char b)
  {
    m_dword = (m_dword & 0xFFFFFF00u) | b;
  }

private:
  unsigned int m_dword{};
};

namespace Colors
{
  static constexpr Color
  MakeRGB(unsigned char r, unsigned char g, unsigned char b)
  {
    return (r << 16) | (g << 8) | b;
  }

  static constexpr Color White{MakeRGB(255u, 255u, 255u)};
  static constexpr Color Black{MakeRGB(0u, 0u, 0u)};

  static constexpr Color Gray{MakeRGB(0x80u, 0x80u, 0x80u)};
  static constexpr Color LightGray{MakeRGB(0xD3u, 0xD3u, 0xD3u)};

  static constexpr Color Red{MakeRGB(255u, 0u, 0u)};
  static constexpr Color Green = {MakeRGB(0u, 255u, 0u)};

  static constexpr Color Blue = {MakeRGB(0u, 0u, 255u)};
  static constexpr Color Yellow = {MakeRGB(255u, 255u, 0u)};

  static constexpr Color Cyan = {MakeRGB(0u, 255u, 255u)};
  static constexpr Color Magenta = {MakeRGB(255u, 0u, 255u)};
} // namespace Colors
#endif // !CHILI_FRAMEWORK_COLOR_H
