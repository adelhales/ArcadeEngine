// Copyright (c) 2025 Adel Hales

#pragma once

#include "Graphics/Effect.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <array>

class EffectBloom : public Effect
{
private:
    sf::Shader downsampleShader_;
    sf::Shader blurShader_;
    sf::Shader additiveShader_;
    std::array<sf::RenderTexture, 2> textures_;

public:
    EffectBloom();

    void Apply(const sf::Texture& input, sf::RenderTarget& output) override;

private:
    void Render(const sf::Shader& shader, sf::RenderTexture& output);
};