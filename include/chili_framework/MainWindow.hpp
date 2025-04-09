/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	MainWindow.h																		  *
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
#ifndef CHILI_FRAMEWORK_MAIN_WINDOW_H
#define CHILI_FRAMEWORK_MAIN_WINDOW_H

#include <chili_framework/ChiliException.hpp>
#include <chili_framework/ChiliWin.hpp>

#include <chili_framework/Keyboard.hpp>
#include <chili_framework/Mouse.hpp>

#include <chili_framework/Graphics.hpp>
#include <string>

class MainWindow
{
public:
  class Exception : public ChiliException
  {
  public:
    using ChiliException::ChiliException;
    virtual std::wstring
    GetFullMessage() const override
    {
      return GetNote() + L"\nAt: " + GetLocation();
    }
    virtual std::wstring
    GetExceptionType() const override
    {
      return L"Windows Exception";
    }
  };

  MainWindow(HINSTANCE hInst, wchar_t* pArgs);
  MainWindow(const MainWindow&) = delete;

  MainWindow&
  operator=(const MainWindow&) = delete;

  ~MainWindow();

  bool
  IsActive() const;

  bool
  IsMinimized() const;

  void
  ShowMessageBoxW(const std::wstring& title,
                  const std::wstring& message,
                  UINT type = MB_OK) const;
  void
  Kill()
  {
    PostQuitMessage(0);
  }

  // returns false if quitting
  bool
  ProcessMessage();

  const std::wstring&
  GetArgs() const
  {
    return m_args;
  }

  HWND
  key() const
  {
    return m_hWnd;
  };

private:
  static LRESULT WINAPI
  _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static LRESULT WINAPI
  _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  LRESULT
  HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
  Keyboard m_kbd{};
  Mouse m_mouse{};

private:
  std::wstring m_args{};
  HWND m_hWnd{};

  HINSTANCE m_hInst{};
  static constexpr wchar_t* m_wndClassName{L"Chili DirectX Framework Window"};
};
#endif // !CHILI_FRAMEWORK_MAIN_WINDOW_H
