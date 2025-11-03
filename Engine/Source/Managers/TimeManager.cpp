// Copyright (c) 2025 Adel Hales

#include "Managers/TimeManager.h"

#include <algorithm>

#include "Core/EngineConfig.h"

void TimeManager::Update()
{
    const sf::Time currentTime = clock_.getElapsedTime();

    deltaTime_ = std::min(currentTime - previousTime_, gConfig.maximumDeltaTime);

    previousTime_ = currentTime;
}

float TimeManager::GetDeltaTime() const
{
    return deltaTime_.asSeconds();
}

float TimeManager::GetElapsedTime() const
{
    return clock_.getElapsedTime().asSeconds();
}