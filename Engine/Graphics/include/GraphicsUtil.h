#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H
#include "ChiliWin/include/ChiliWin.h"

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
