// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

struct EngineConfig
{
    std::string windowTitle;
    sf::Vector2f windowSize;
    bool disableSfmlLogs;
    sf::Time maximumDeltaTime;
    float globalVolume;
    sf::Color backgroundColor;
    float cursorRadius;
    float cursorSpeed;
    sf::Color cursorColor;
    float joystickDeadzone;

    EngineConfig();
};

inline const EngineConfig gConfig;