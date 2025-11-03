// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Window/Event.hpp>

struct EngineVisitor
{
    class Engine& engine;

    void operator()(const sf::Event::Closed&);
    void operator()(const sf::Event::Resized&);
    void operator()(const sf::Event::FocusLost&);
    void operator()(const sf::Event::FocusGained&);
    void operator()(const sf::Event::JoystickConnected&);
    void operator()(const sf::Event::JoystickDisconnected&);
    void operator()(const sf::Event::KeyPressed&);
    void operator()(const sf::Event::JoystickButtonPressed&);
    void operator()(const auto&) {};
};