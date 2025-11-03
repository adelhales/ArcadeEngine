// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class CursorManager
{
private:
    sf::RenderWindow& window_;
    sf::CircleShape shape_;
    float speed_;
    bool visible_;

public:
    CursorManager(sf::RenderWindow& window);

    void SetSpeed(float speed);
    float GetSpeed() const;

    void SetVisible(bool visible);
    bool IsVisible() const;

    void SetPosition(sf::Vector2f position);
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetPosition(const sf::View& view) const;

private:
    friend class Engine;

    void Update(float deltaTime);
    void Render() const;
};