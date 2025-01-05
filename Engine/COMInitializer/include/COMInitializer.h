#ifndef CONINITIALIZER_H
#define CONINITIALIZER_H
#include "ChiliWin/include/ChiliWin.h"

class COMInitializer
{
public:
  COMInitializer();

  ~COMInitializer();

private:
  HRESULT m_hr{};
};
#endif // !CONINITIALIZER_H
