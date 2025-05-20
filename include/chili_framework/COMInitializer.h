#pragma once

#include <chili_framework/ChiliWin.h>

class COMInitializer
{
public:
  COMInitializer();
  ~COMInitializer();

private:
  HRESULT hr;
};
