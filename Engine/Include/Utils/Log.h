// Copyright (c) 2025 Adel Hales

#pragma once

#include <spdlog/spdlog.h>

#define LOG_INFO(...)    SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARNING(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)   SPDLOG_ERROR(__VA_ARGS__)