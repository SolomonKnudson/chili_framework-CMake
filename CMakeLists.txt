cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

project(chili_framework-CMake LANGUAGES CXX)

add_library(chili_framework)
add_library(chili_framework-CMake::chili_framework ALIAS chili_framework)

target_sources(chili_framework PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/D3PipeLine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/D3PipeLineCreate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/D3PipeLineInit.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Graphics.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/GraphicsException.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/GraphicsFrame.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/GraphicsUtil.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/src/COMInitializer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/DXErr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Keyboard.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/MainWindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Mouse.cpp
)

target_include_directories(chili_framework PUBLIC
  $<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(chili_framework PUBLIC d3d11.lib)
add_dependencies(chili_framework d3d11.lib)

target_compile_features(chili_framework PUBLIC cxx_std_17)
set_target_properties(chili_framework PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED on
  CXX_EXTENSIONS off
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FrameBuffers)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Resource)
