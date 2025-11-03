// Copyright (c) 2025 Adel Hales

#include "Graphics/Effects/EffectBloom.h"

#include <SFML/Graphics/Sprite.hpp>

#include <string>

#include "Core/EngineConfig.h"
#include "Utils/Verify.h"

EffectBloom::EffectBloom()
{
    const std::string shadersPath  = "Content/Shaders/";
    const std::string vertexShader = shadersPath + "Default.vert";

    VERIFY(downsampleShader_.loadFromFile(vertexShader, shadersPath + "Downsample.frag"));
    VERIFY(blurShader_.      loadFromFile(vertexShader, shadersPath + "Blur.frag"));
    VERIFY(additiveShader_.  loadFromFile(vertexShader, shadersPath + "Additive.frag"));

    // Half-size ping-pong textures for faster and smoother blur
    for (sf::RenderTexture& texture : textures_)
    {
        VERIFY(texture.resize(sf::Vector2u(gConfig.windowSize / 2.f)));
    }
}

void EffectBloom::Apply(const sf::Texture& input, sf::RenderTarget& output)
{
    // 1. Downsample to reduce input resolution before blur
    downsampleShader_.setUniform("sourceTexture", input);
    downsampleShader_.setUniform("texelSize", sf::Vector2f(1.f / input.getSize().x, 1.f / input.getSize().y));
    Render(downsampleShader_, textures_[0]);

    // 2. Vertical gaussian blur to spread brightness downward
    blurShader_.setUniform("sourceTexture", textures_[0].getTexture());
    blurShader_.setUniform("texelSize", sf::Vector2f(0, 1.f / textures_[0].getSize().y));
    Render(blurShader_, textures_[1]);

    // 3. Horizontal gaussian blur to spread brightness sideways
    blurShader_.setUniform("sourceTexture", textures_[1].getTexture());
    blurShader_.setUniform("texelSize", sf::Vector2f(1.f / textures_[1].getSize().x, 0));
    Render(blurShader_, textures_[0]);

    // 4. Add bloom on top of original image
    additiveShader_.setUniform("sourceTexture", sf::Shader::CurrentTexture);
    additiveShader_.setUniform("bloomTexture", textures_[0].getTexture());
    output.draw(sf::Sprite(input), &additiveShader_);
}

void EffectBloom::Render(const sf::Shader& shader, sf::RenderTexture& output)
{
    const sf::Vector2f outputSize(output.getSize());

    const sf::Vertex quad[] = {
        {{0, 0},            sf::Color::White, {0, 1}},
        {{outputSize.x, 0}, sf::Color::White, {1, 1}},
        {{0, outputSize.y}, sf::Color::White, {0, 0}},
        {outputSize,        sf::Color::White, {1, 0}},
    };

    sf::RenderStates states(&shader);
    states.blendMode = sf::BlendNone;

    output.draw(quad, 4, sf::PrimitiveType::TriangleStrip, states);
    output.display();
}