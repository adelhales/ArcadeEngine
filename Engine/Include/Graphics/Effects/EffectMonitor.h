// Copyright (c) 2025 Adel Hales

#pragma once

#include "Graphics/Effect.h"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>

class EffectMonitor : public Effect
{
private:
    sf::Shader shader_;
    sf::Clock clock_;

public:
    EffectMonitor();

    void Apply(const sf::Texture& input, sf::RenderTarget& output) override;
};