// Copyright (c) 2025 Adel Hales

#pragma once

#include <nlohmann/json.hpp>

#include <string_view>

class SaveManager
{
private:
    nlohmann::json values_;

public:
    SaveManager();
    ~SaveManager();

    void Set(std::string_view key, auto value)
    {
        values_[key] = value;
    }

    auto Get(std::string_view key, auto defaultValue = {}) const
    {
        return values_.value(key, defaultValue);
    }

    bool Has(std::string_view key) const;
    void Erase(std::string_view key);
    void Clear();
};