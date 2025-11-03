// Copyright (c) 2025 Adel Hales

#include "Managers/RenderManager.h"

#include <utility>

#include "Graphics/Effects/EffectBloom.h"
#include "Graphics/Effects/EffectMonitor.h"

#include "Core/EngineConfig.h"
#include "Utils/Verify.h"

RenderManager::RenderManager() :
    target_(sf::Vector2u(gConfig.windowSize)),
    backgroundTexture_("Content/Textures/Background.png"),
    background_(gConfig.windowSize)
{
    target_.setSmooth(true);

    background_.setTexture(&backgroundTexture_);
    background_.setFillColor(gConfig.backgroundColor);

    if (sf::Shader::isAvailable())
    {
        VERIFY(effectsTarget_.resize(sf::Vector2u(gConfig.windowSize)));

        effects_.emplace_back(std::make_unique<EffectBloom>());
        effects_.emplace_back(std::make_unique<EffectMonitor>());
    }
}

void RenderManager::BeginDrawing()
{
    target_.clear();

    Draw(background_);
}

const sf::Texture& RenderManager::FinishDrawing()
{
    target_.display();

    sf::RenderTexture* input  = &target_;
    sf::RenderTexture* output = &effectsTarget_;

    for (auto& effect : effects_)
    {
        output->clear();
        effect->Apply(input->getTexture(), *output);
        output->display();

        std::swap(input, output);
    }

    return input->getTexture();
}

void RenderManager::Draw(const sf::Drawable& drawable)
{
    target_.draw(drawable);
}

void RenderManager::Draw(std::span<sf::Vertex> vertices, sf::PrimitiveType type)
{
    target_.draw(vertices.data(), vertices.size(), type);
}

void RenderManager::SetView(const sf::View& view)
{
    target_.setView(view);
}

void RenderManager::ResetView()
{
    SetView(target_.getDefaultView());
}