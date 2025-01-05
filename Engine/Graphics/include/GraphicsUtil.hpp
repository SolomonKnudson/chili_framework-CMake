#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H
#include "ChiliWin/include/ChiliWin.hpp"

#define CHILI_GFX_EXCEPTION(hr, note)                                          \
  Graphics::Exception(hr, note, _CRT_WIDE(__FILE__), __LINE__)

namespace GraphicsUtil
{
  constexpr bool
  succeeded(HRESULT hr)
  {
    return hr >= 0;
  }

  constexpr bool
  failed(const HRESULT hr)
  {
    return hr < 0;
  }
} // namespace GraphicsUtil
#endif // !GRAPHICS_UTIL_H
