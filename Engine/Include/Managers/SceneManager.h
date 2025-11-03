// Copyright (c) 2025 Adel Hales

#pragma once

#include <optional>
#include <string>
#include <string_view>

class SceneManager
{
private:
    std::string currentScene_;
    std::optional<std::string> nextScene_;

public:
    void ChangeScene(std::string_view name);
    void RestartCurrentScene();

private:
    friend class Engine;

    std::optional<std::string> FetchNextScene();
};