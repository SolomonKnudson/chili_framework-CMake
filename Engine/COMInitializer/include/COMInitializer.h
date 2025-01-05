#pragma once

#include "ChiliWin/include/ChiliWin.h"

class COMInitializer
{
public:
  COMInitializer();

  ~COMInitializer();

private:
  HRESULT m_hr{};
};
