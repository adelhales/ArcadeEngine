// Copyright (c) 2025 Adel Hales

#include "Graphics/Effects/EffectGlitch.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include "Utils/Verify.h"

EffectGlitch::EffectGlitch()
{
    VERIFY(shader_.loadFromFile("Content/Shaders/Glitch.frag", sf::Shader::Type::Fragment));
}

void EffectGlitch::Apply(const sf::Texture& input, sf::RenderTarget& output)
{
    shader_.setUniform("sourceTexture", sf::Shader::CurrentTexture);
    shader_.setUniform("time", clock_.getElapsedTime().asSeconds());

    output.draw(sf::Sprite(input), &shader_);
}