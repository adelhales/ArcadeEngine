// Copyright (c) 2025 Adel Hales

#include "Managers/CursorManager.h"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>

#include "Core/EngineConfig.h"

CursorManager::CursorManager(sf::RenderWindow& window) :
    window_(window),
    shape_(gConfig.cursorRadius),
    speed_(gConfig.cursorSpeed),
    visible_(true)
{
    shape_.setFillColor(gConfig.cursorColor);
    shape_.setOrigin(shape_.getGeometricCenter());
    shape_.setPosition(gConfig.windowSize / 2.f);
}

void CursorManager::Update(float deltaTime)
{
    const sf::Vector2f joystickDirection(
        sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100,
        sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100
    );

    if (joystickDirection.length() > gConfig.joystickDeadzone)
    {
        shape_.move(joystickDirection * speed_ * deltaTime);
        shape_.setPosition({ std::clamp(shape_.getPosition().x, 0.f, gConfig.windowSize.x - 1),
                             std::clamp(shape_.getPosition().y, 0.f, gConfig.windowSize.y - 1)});

        SetPosition(shape_.getPosition());
    }
    else
    {
        shape_.setPosition(GetPosition());
    }
}

void CursorManager::Render() const
{
    if (visible_)
    {
        window_.draw(shape_);
    }
}

void CursorManager::SetSpeed(float speed)
{
    speed_ = speed;
}

float CursorManager::GetSpeed() const
{
    return speed_;
}

void CursorManager::SetVisible(bool visible)
{
    visible_ = visible;
}

bool CursorManager::IsVisible() const
{
    return visible_;
}

void CursorManager::SetPosition(sf::Vector2f position)
{
    sf::Mouse::setPosition(window_.mapCoordsToPixel(position, window_.getDefaultView()), window_);
}

sf::Vector2f CursorManager::GetPosition() const
{
    return GetPosition(window_.getDefaultView());
}

sf::Vector2f CursorManager::GetPosition(const sf::View& view) const
{
    return window_.mapPixelToCoords(sf::Mouse::getPosition(window_), view);
}