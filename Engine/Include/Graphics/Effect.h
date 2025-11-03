// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

class Effect
{
public:
    virtual ~Effect() = default;

    virtual void Apply(const sf::Texture& input, sf::RenderTarget& output) = 0;
};