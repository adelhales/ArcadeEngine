// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/System/Vector3.hpp>

class AudioManager
{
public:
    void SetGlobalVolume(float volume);
    float GetGlobalVolume() const;

    void SetListenerPosition(sf::Vector3f position);
    sf::Vector3f GetListenerPosition() const;

    void SetListenerDirection(sf::Vector3f direction);
    sf::Vector3f GetListenerDirection() const;
};