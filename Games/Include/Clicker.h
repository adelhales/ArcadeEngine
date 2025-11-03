// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/ClickerTypes.h"

namespace Clicker
{
    class Game : public Scene
    {
    private:
        Target target;
        Stats stats;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitTarget();
        void InitStats();

        void BindInputs();

        void StartTarget();
        void StartStats();

        void UpdateStats();
        void UpdateTarget();
        void UpdateTargetColor();

        void EventTargetClicked();
        void EventTargetTeleport();
        void EventStatsScoreIncrease();

        bool IsTargetHovered() const;
    };
}