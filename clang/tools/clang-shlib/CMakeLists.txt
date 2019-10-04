# Building libclang-cpp.so fails if LLVM_ENABLE_PIC=Off
if (NOT LLVM_ENABLE_PIC)
  return()
endif()

get_property(clang_libs GLOBAL PROPERTY CLANG_STATIC_LIBS)

foreach (lib ${clang_libs})
  list(APPEND _OBJECTS $<TARGET_OBJECTS:obj.${lib}>)
  list(APPEND _DEPS $<TARGET_PROPERTY:${lib},INTERFACE_LINK_LIBRARIES>)
  get_target_property(interface ${lib} LINK_LIBRARIES)
  list(APPEND _DEPS ${interface})
endforeach ()

# clang libraries are redundant since we are linking all the individual
# object files into libclang-cpp.so, so filter them out from _DEPS.
# This avoids problems with LLVM global data when building with
# BUILD_SHARED_LIBS=ON
list(FILTER _DEPS EXCLUDE REGEX "^clang")
if (CLANG_LINK_CLANG_DYLIB)
  set(INSTALL_WITH_TOOLCHAIN INSTALL_WITH_TOOLCHAIN)
endif()

add_clang_library(clang-cpp
                  SHARED
                  ${INSTALL_WITH_TOOLCHAIN}
                  clang-shlib.cpp
                  ${_OBJECTS}
                  LINK_LIBS
                  ${_DEPS})
