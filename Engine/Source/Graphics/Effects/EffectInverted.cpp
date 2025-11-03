// Copyright (c) 2025 Adel Hales

#include "Graphics/Effects/EffectInverted.h"

#include <SFML/Graphics/Sprite.hpp>

#include "Utils/Verify.h"

EffectInverted::EffectInverted()
{
    VERIFY(shader_.loadFromFile("Content/Shaders/Invert.frag", sf::Shader::Type::Fragment));
}

void EffectInverted::Apply(const sf::Texture& input, sf::RenderTarget& output)
{
    shader_.setUniform("sourceTexture", sf::Shader::CurrentTexture);

    output.draw(sf::Sprite(input), &shader_);
}