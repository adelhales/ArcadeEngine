// Copyright (c) 2025 Adel Hales

#include "Managers/SceneManager.h"

#include <utility>

#include "Utils/Log.h"

std::optional<std::string> SceneManager::FetchNextScene()
{
    if (nextScene_)
    {
        if (currentScene_ == *nextScene_)
        {
            LOG_INFO("Restarting Scene: {}", currentScene_);
        }
        else
        {
            LOG_INFO("Changing Scene to: {}", *nextScene_);
        }

        currentScene_ = *nextScene_;
        return std::exchange(nextScene_, std::nullopt);
    }

    return std::nullopt;
}

void SceneManager::ChangeScene(std::string_view name)
{
    if (!nextScene_)
    {
        nextScene_ = name;
    }
}

void SceneManager::RestartCurrentScene()
{
    ChangeScene(currentScene_);
}