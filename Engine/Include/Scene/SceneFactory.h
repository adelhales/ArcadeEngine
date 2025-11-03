// Copyright (c) 2025 Adel Hales

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Core/EngineContext.h"
#include "Scene/Scene.h"

namespace SceneFactory
{
    using Scenes = std::unordered_map<std::string, std::unique_ptr<Scene>>;

    Scenes CreateScenes(EngineContext& context);
}