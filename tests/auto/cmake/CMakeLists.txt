
cmake_minimum_required(VERSION 2.8)

project(qmake_cmake_files)

enable_testing()

find_package(Qt5Mvc REQUIRED)

include("${_Qt5CTestMacros}")

test_module_includes(
  Mvc GMvc
)
