// Copyright (c) 2025 Adel Hales

#pragma once

#include <cassert>

#ifdef NDEBUG
    #define VERIFY(expr) void(expr)
#else
    #define VERIFY(expr) assert(expr)
#endif