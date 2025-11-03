// Copyright (c) 2025 Adel Hales

#include "Managers/SaveManager.h"

#include <fstream>

SaveManager::SaveManager()
{
    if (std::ifstream file{"Content/Save.json"})
    {
        values_ = nlohmann::json::parse(file);
    }
    else
    {
        values_ = nlohmann::json::object();
    }
}

SaveManager::~SaveManager()
{
    if (!values_.empty())
    {
        if (std::ofstream file{"Content/Save.json"})
        {
            file << values_.dump(4);
        }
    }
}

bool SaveManager::Has(std::string_view key) const
{
    return values_.contains(key);
}

void SaveManager::Erase(std::string_view key)
{
    values_.erase(key);
}

void SaveManager::Clear()
{
    values_.clear();
}