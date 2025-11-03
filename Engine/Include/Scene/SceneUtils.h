// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <ranges>
#include <vector>

#include "Core/EngineConfig.h"
#include "Utils/Cooldown.h"
#include "Utils/Log.h"
#include "Utils/Verify.h"

inline const sf::Font& GetDefaultFont()
{
    static const sf::Font font("Content/Fonts/Montserrat.ttf");
    return font;
}

inline bool IsOutsideWindowLeft(const sf::Shape& shape)
{
    return shape.getPosition().x < shape.getGlobalBounds().size.x / 2;
}

inline bool IsOutsideWindowRight(const sf::Shape& shape)
{
    return shape.getPosition().x > gConfig.windowSize.x - shape.getGlobalBounds().size.x / 2;
}

inline bool IsOutsideWindowTop(const sf::Shape& shape)
{
    return shape.getPosition().y < shape.getGlobalBounds().size.y / 2;
}

inline bool IsOutsideWindowBottom(const sf::Shape& shape)
{
    return shape.getPosition().y > gConfig.windowSize.y - shape.getGlobalBounds().size.y / 2;
}

inline bool IsOutsideWindow(sf::Vector2f point)
{
    return point.x < 0 || point.y < 0 || point.x >= gConfig.windowSize.x || point.y >= gConfig.windowSize.y;
}

inline std::optional<sf::FloatRect> Intersects(const sf::Shape& shape1, const sf::Shape& shape2)
{
    return shape1.getGlobalBounds().findIntersection(shape2.getGlobalBounds());
}

inline float DistanceSquared(sf::Vector2f point1, sf::Vector2f point2)
{
    return (point1 - point2).lengthSquared();
}

inline float Distance(sf::Vector2f point1, sf::Vector2f point2)
{
    return (point1 - point2).length();
}

inline bool Contains(const sf::Shape& shape, sf::Vector2f point)
{
    return shape.getGlobalBounds().contains(point);
}

inline bool Contains(const sf::CircleShape& circle, sf::Vector2f point)
{
    return DistanceSquared(circle.getPosition(), point) <= std::pow(circle.getRadius(), 2);
}

inline sf::Vector2f Lerp(sf::Vector2f start, sf::Vector2f end, float t)
{
    return {std::lerp(start.x, end.x, t), std::lerp(start.y, end.y, t)};
}