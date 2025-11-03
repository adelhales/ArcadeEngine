// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Window/Event.hpp>

#include "Core/EngineContext.h"
#include "Scene/SceneUtils.h"

class Scene
{
protected:
    EngineContext& ctx;

public:
    Scene(EngineContext& context) : ctx(context) {}
    virtual ~Scene() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Render() const = 0;

    virtual void OnEvent(const sf::Event&) {};
    virtual void OnPause(bool /* paused */) {}
    virtual void OnCleanup() {};
};