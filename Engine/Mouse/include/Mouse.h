/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Mouse.h																				  *
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
#ifndef MOUSE_H
#define MOUSE_H

#include <queue>

class Mouse
{
  friend class MainWindow;

public:
  class Event
  {
  public:
    enum class Type
    {
      LPress,
      LRelease,
      RPress,
      RRelease,
      WheelUp,
      WheelDown,
      Move,
      Invalid
    };

    Event()
      : m_type{Type::Invalid}
      , m_leftIsPressed{}
      , m_rightIsPressed{}
      , m_x{}
      , m_y{}
    {
    }
    Event(Type type, const Mouse& parent)
      : m_type{type}
      , m_leftIsPressed{parent.leftIsPressed}
      , m_rightIsPressed{parent.rightIsPressed}
      , m_x{parent.x}
      , m_y{parent.y}
    {
    }

    bool
    IsValid() const
    {
      return m_type != Type::Invalid;
    }

    Type
    GetType() const
    {
      return m_type;
    }

    std::pair<int, int>
    GetPos() const
    {
      return {m_x, m_y};
    }

    int
    GetPosX() const
    {
      return m_x;
    }

    int
    GetPosY() const
    {
      return m_y;
    }

    bool
    LeftIsPressed() const
    {
      return m_leftIsPressed;
    }

    bool
    RightIsPressed() const
    {
      return m_rightIsPressed;
    }

  private:
    Type m_type{};
    bool m_leftIsPressed{};
    bool m_rightIsPressed{};
    int m_x{};
    int m_y{};
  };

public:
  Mouse() = default;
  Mouse(const Mouse&) = delete;
  Mouse&
  operator=(const Mouse&) = delete;
  std::pair<int, int>
  GetPos() const;
  int
  GetPosX() const;
  int
  GetPosY() const;
  bool
  LeftIsPressed() const;
  bool
  RightIsPressed() const;
  bool
  IsInWindow() const;
  Mouse::Event
  Read();
  bool
  IsEmpty() const
  {
    return buffer.empty();
  }
  void
  Flush();

private:
  void
  OnMouseMove(int x, int y);
  void
  OnMouseLeave();
  void
  OnMouseEnter();
  void
  OnLeftPressed(int x, int y);
  void
  OnLeftReleased(int x, int y);
  void
  OnRightPressed(int x, int y);
  void
  OnRightReleased(int x, int y);
  void
  OnWheelUp(int x, int y);
  void
  OnWheelDown(int x, int y);
  void
  TrimBuffer();

private:
  std::queue<Event> buffer{};
  static constexpr unsigned int bufferSize{4u};
  int x{};
  int y{};

  bool leftIsPressed{false};
  bool rightIsPressed{false};

  bool isInWindow{};
};
#endif // !MOUSE_H
