// Copyright (c) 2025 Adel Hales

#include "Managers/AudioManager.h"

#include <SFML/Audio/Listener.hpp>

void AudioManager::SetGlobalVolume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
}

float AudioManager::GetGlobalVolume() const
{
    return sf::Listener::getGlobalVolume();
}

void AudioManager::SetListenerPosition(sf::Vector3f position)
{
    sf::Listener::setPosition(position);
}

sf::Vector3f AudioManager::GetListenerPosition() const
{
    return sf::Listener::getPosition();
}

void AudioManager::SetListenerDirection(sf::Vector3f direction)
{
    sf::Listener::setDirection(direction);
}

sf::Vector3f AudioManager::GetListenerDirection() const
{
    return sf::Listener::getDirection();
}