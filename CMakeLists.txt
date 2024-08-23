cmake_minimum_required(VERSION 3.26)

project(alloca-type-infer)

# Configure LLVM
find_package(LLVM 18.1.8 REQUIRED CONFIG)

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_INSTALL_LIBDIR}/)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_INSTALL_LIBDIR}/)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
include(AddLLVM)

message(STATUS "LLVM_DIR IS ${LLVM_CMAKE_DIR}.")

# Compile the project.
include_directories(
  include
  ${LLVM_INCLUDE_DIRS}
)

file(GLOB_RECURSE sources "src/*.cpp" "src/*.c" "*.cc")

add_llvm_library(
  alloca-type-infer

  MODULE
  ${sources}
)

set_property(
  TARGET alloca-type-infer
  PROPERTY
  CXX_STANDARD 17
)

install(
  TARGETS alloca-type-infer
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
