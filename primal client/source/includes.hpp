#pragma once

#define M_PI 3.141592653589793238
#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];

#include <string>
#include <vector>
#include <locale>
#include <array>
#include <Windows.h>
#include <algorithm>
#include "sk_crypter.hpp"
#include <map>