/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	MainWindow.cpp																		  *
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
#include "MainWindow/include/MainWindow.hpp"
#include "Resource/include/Resource.hpp"
#include <assert.h>

MainWindow::MainWindow(HINSTANCE hInst, wchar_t* pArgs)
  : m_mouse{}
  , m_kbd{}
  , m_args{pArgs}
  , m_hInst{hInst}
{
  // register window class
  WNDCLASSEX wc = {sizeof(WNDCLASSEX),
                   CS_CLASSDC,
                   _HandleMsgSetup,
                   0,
                   0,
                   hInst,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   (LPCSTR)(m_wndClassName),
                   nullptr};
  wc.hIconSm = (HICON)LoadImage(
      hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0);
  wc.hIcon = (HICON)LoadImage(
      hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0);
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  RegisterClassEx(&wc);

  // create window & get hWnd
  RECT wr;
  wr.left = 350;
  wr.right = Graphics::ScreenWidth + wr.left;
  wr.top = 100;
  wr.bottom = Graphics::ScreenHeight + wr.top;
  AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
  m_hWnd = CreateWindow((LPCSTR)m_wndClassName,
                        "Chili DirectX Framework",
                        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                        wr.left,
                        wr.top,
                        wr.right - wr.left,
                        wr.bottom - wr.top,
                        nullptr,
                        nullptr,
                        hInst,
                        this);

  // throw exception if something went terribly wrong
  if (m_hWnd == nullptr)
  {
    throw Exception(
        _CRT_WIDE(__FILE__), __LINE__, L"Failed to get valid window handle.");
  }

  // show and update
  ShowWindow(m_hWnd, SW_SHOWDEFAULT);
  UpdateWindow(m_hWnd);
}

MainWindow::~MainWindow()
{
  // unregister window class
  UnregisterClass((LPCSTR)m_wndClassName, m_hInst);
}

bool
MainWindow::IsActive() const
{
  return GetActiveWindow() == m_hWnd;
}

bool
MainWindow::IsMinimized() const
{
  return IsIconic(m_hWnd) != 0;
}

void
MainWindow::ShowMessageBox(const std::wstring& title,
                           const std::wstring& message,
                           UINT type) const
{
  MessageBox(m_hWnd, (LPCSTR)message.c_str(), (LPCSTR)title.c_str(), type);
}

bool
MainWindow::ProcessMessage()
{
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
    {
      return false;
    }
  }
  return true;
}

LRESULT WINAPI
MainWindow::_HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
  if (msg == WM_NCCREATE)
  {
    // extract ptr to window class from creation data
    const CREATESTRUCTW* const pCreate =
        reinterpret_cast<CREATESTRUCTW*>(lParam);
    MainWindow* const pWnd =
        reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
    // sanity check
    assert(pWnd != nullptr);
    // set WinAPI-managed user data to store ptr to window class
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    // set message proc to normal (non-setup) handler now that setup is finished
    SetWindowLongPtr(hWnd,
                     GWLP_WNDPROC,
                     reinterpret_cast<LONG_PTR>(&MainWindow::_HandleMsgThunk));
    // forward message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
  }
  // if we get a message before the WM_NCCREATE message, handle with default handler
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI
MainWindow::_HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // retrieve ptr to window class
  MainWindow* const pWnd =
      reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  // forward message to window class handler
  return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT
MainWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    case WM_KILLFOCUS:
      m_kbd.ClearState();
      break;

      // ************ KEYBOARD MESSAGES ************ //
    case WM_KEYDOWN:
      if (!(lParam & 0x40000000) ||
          m_kbd.AutorepeatIsEnabled()) // no thank you on the autorepeat
      {
        m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
      }
      break;

    case WM_KEYUP:
      m_kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
      break;

    case WM_CHAR:
      m_kbd.OnChar(static_cast<unsigned char>(wParam));
      break;
      // ************ END KEYBOARD MESSAGES ************ //

      // ************ MOUSE MESSAGES ************ //
    case WM_MOUSEMOVE:
      {
        POINTS pt = MAKEPOINTS(lParam);
        if (pt.x > 0 && pt.x < Graphics::ScreenWidth && pt.y > 0 &&
            pt.y < Graphics::ScreenHeight)
        {
          m_mouse.OnMouseMove(pt.x, pt.y);
          if (!m_mouse.IsInWindow())
          {
            SetCapture(hWnd);
            m_mouse.OnMouseEnter();
          }
        }
        else
        {
          if (wParam & (MK_LBUTTON | MK_RBUTTON))
          {
            pt.x = std::max(short(0), pt.x);
            pt.x = std::min(short(Graphics::ScreenWidth - 1), pt.x);
            pt.y = std::max(short(0), pt.y);
            pt.y = std::min(short(Graphics::ScreenHeight - 1), pt.y);
            m_mouse.OnMouseMove(pt.x, pt.y);
          }
          else
          {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
            m_mouse.OnLeftReleased(pt.x, pt.y);
            m_mouse.OnRightReleased(pt.x, pt.y);
          }
        }
        break;
      }
    case WM_LBUTTONDOWN:
      {
        const POINTS pt = MAKEPOINTS(lParam);
        m_mouse.OnLeftPressed(pt.x, pt.y);
        SetForegroundWindow(hWnd);
        break;
      }
    case WM_RBUTTONDOWN:
      {
        const POINTS pt = MAKEPOINTS(lParam);
        m_mouse.OnRightPressed(pt.x, pt.y);
        break;
      }
    case WM_LBUTTONUP:
      {
        const POINTS pt = MAKEPOINTS(lParam);
        m_mouse.OnLeftReleased(pt.x, pt.y);
        break;
      }
    case WM_RBUTTONUP:
      {
        const POINTS pt = MAKEPOINTS(lParam);
        m_mouse.OnRightReleased(pt.x, pt.y);
        break;
      }
    case WM_MOUSEWHEEL:
      {
        const POINTS pt = MAKEPOINTS(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
          m_mouse.OnWheelUp(pt.x, pt.y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
          m_mouse.OnWheelDown(pt.x, pt.y);
        }
        break;
      }
      // ************ END MOUSE MESSAGES ************ //
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
