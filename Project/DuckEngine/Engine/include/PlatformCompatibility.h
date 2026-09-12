#pragma once

// The original engine headers use MSVC's __declspec directly. Keep their
// Windows ABI unchanged while allowing the same headers to compile on other
// toolchains. This header is force-included by the CMake target on non-MSVC
// builds so the existing Visual Studio projects remain untouched.
#ifndef _WIN32
#ifndef __declspec
#define __declspec(attribute)
#endif
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(parameter) (void)(parameter)
#endif
