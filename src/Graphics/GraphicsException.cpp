/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	GraphicsException.cpp																		  *
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
#include <chili_framework/DXErr.hpp>
#include <chili_framework/Graphics.hpp>

#include <array>
#include <string>

GraphicsException::GraphicsException(HRESULT hr,
                                     const std::wstring& note,
                                     const wchar_t* file,
                                     unsigned int line)
  : ChiliException{file, line, note}
  , m_hr{hr}
{
}

std::wstring
GraphicsException::GraphicsException::GetFullMessage() const
{
  const std::wstring empty{L""};
  const std::wstring errorName{GetErrorName()};

  const std::wstring errorDesc{GetErrorDescription()};
  const std::wstring& note{GetNote()};

  const std::wstring location{GetLocation()};

  return (!errorName.empty() ? std::wstring(L"Error: ") + errorName + L"\n"
                             : empty) +
         (!errorDesc.empty()
              ? std::wstring(L"Description: ") + errorDesc + L"\n"
              : empty) +
         (!note.empty() ? std::wstring(L"Note: ") + note + L"\n" : empty) +
         (!location.empty() ? std::wstring(L"Location: ") + location : empty);
}

std::wstring
GraphicsException::GetErrorName() const
{
  return DXGetErrorString(m_hr);
}

std::wstring
GraphicsException::GetErrorDescription() const
{
  std::wstring wideDescription{};
  wideDescription.reserve(512);

  DXGetErrorDescription(m_hr, wideDescription.data(), wideDescription.size());

  return wideDescription;
}

std::wstring
GraphicsException::GetExceptionType() const
{
  return L"Chili Graphics Exception";
}
