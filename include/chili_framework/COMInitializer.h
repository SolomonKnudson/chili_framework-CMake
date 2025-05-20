#pragma once

#include <chili_framework/ChiliWin.hpp>

class COMInitializer
{
public:
  COMInitializer();
  ~COMInitializer();

private:
  HRESULT hr;
};
