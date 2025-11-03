// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/EngineContext.h"
#include "Core/EngineVisitor.h"
#include "Core/Overlay.h"
#include "Scene/SceneFactory.h"

class Engine
{
private:
    sf::RenderWindow window_;
    EngineContext context_;

    SceneFactory::Scenes scenes_;
    Scene* currentScene_;

    Overlay overlay_;
    bool cursorWasVisible_;

public:
    Engine();

    bool IsRunning() const;
    bool HasFocus() const;

    void ProcessEvents();
    void Update();
    void Render();

private:
    friend EngineVisitor;

    void EventWindowClose();
    void EventWindowResized(sf::Vector2u size);
    void EventWindowFocusLost();
    void EventWindowFocusGained();
    void EventWindowScreenshot() const;
    void EventGamepadConnected(int id);
    void EventGamepadDisconnected(int id);
    void EventSceneChange(const std::string& name);
    void EventSceneRestart();
    void EventSceneMenuReturn();
    void EventOverlayPauseToggle();
    void EventOverlaySelect(OverlaySelection selection);
};