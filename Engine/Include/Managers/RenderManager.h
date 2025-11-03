// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <span>
#include <vector>

#include "Graphics/Effect.h"

class RenderManager
{
private:
    sf::RenderTexture target_;

    sf::Texture backgroundTexture_;
    sf::RectangleShape background_;

    std::vector<std::unique_ptr<Effect>> effects_;
    sf::RenderTexture effectsTarget_;

public:
    RenderManager();

    void Draw(const sf::Drawable& drawable);
    void Draw(std::span<sf::Vertex> vertices, sf::PrimitiveType type);

    void SetView(const sf::View& view);
    void ResetView();

private:
    friend class Engine;

    void BeginDrawing();
    const sf::Texture& FinishDrawing();
};