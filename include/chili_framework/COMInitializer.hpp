#ifndef CHILI_FRAMEWORK_CONINITIALIZER_H
#define CHILI_FRAMEWORK_CONINITIALIZER_H
#include <chili_framework/ChiliWin.hpp>

class COMInitializer
{
public:
  COMInitializer();

  ~COMInitializer();

private:
  HRESULT m_hr{};
};
#endif // !CHILI_FRAMEWORK_CONINITIALIZER_H
