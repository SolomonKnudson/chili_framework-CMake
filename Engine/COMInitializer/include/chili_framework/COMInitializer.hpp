#ifndef CONINITIALIZER_H
#define CONINITIALIZER_H
#include <chili_framework/ChiliWin.hpp>

class COMInitializer
{
public:
  COMInitializer();

  ~COMInitializer();

private:
  HRESULT m_hr{};
};
#endif // !CONINITIALIZER_H
