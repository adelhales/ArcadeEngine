// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class TimeManager
{
private:
    sf::Clock clock_;
    sf::Time deltaTime_;
    sf::Time previousTime_;

public:
    float GetDeltaTime() const;
    float GetElapsedTime() const;

private:
    friend class Engine;

    void Update();
};