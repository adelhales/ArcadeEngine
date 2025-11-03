// Copyright (c) 2025 Adel Hales

#include "Managers/RandomManager.h"

int RandomManager::Int(int min, int max)
{
    return std::uniform_int_distribution(min, max)(generator_);
}

float RandomManager::Float(float min, float max)
{
    return std::uniform_real_distribution(min, max)(generator_);
}

bool RandomManager::Bool(float probability)
{
    return std::bernoulli_distribution(probability)(generator_);
}

sf::Vector2f RandomManager::Position(sf::Vector2f min, sf::Vector2f max)
{
    return {Float(min.x, max.x), Float(min.y, max.y)};
}

sf::Angle RandomManager::Angle(sf::Angle min, sf::Angle max)
{
    return sf::degrees(Float(min.asDegrees(), max.asDegrees()));
}

sf::Color RandomManager::Color(sf::Color min, sf::Color max)
{
    return {(uint8_t)Int(min.r, max.r), (uint8_t)Int(min.g, max.g), (uint8_t)Int(min.b, max.b)};
}