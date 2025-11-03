// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/RunnerTypes.h"

#include <deque>

namespace Runner
{
    class Game : public Scene
    {
    private:
        Player player;
        std::deque<Obstacle> obstacles;
        Stats stats;
        sf::RectangleShape ground;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;

    private:
        void InitPlayer();
        void InitStats();
        void InitGround();

        void BindInputs();

        void StartPlayer();
        void StartStats();

        void UpdatePlayer();
        void UpdatePlayerMove();
        void UpdatePlayerSize();
        void UpdatePlayerGrounded();
        void UpdateObstacles();
        void UpdateObstaclesSpawn();
        void UpdateObstaclesMove();
        void UpdateObstaclesPopFront();
        void UpdateStats();

        void EventPlayerJump();
        void EventObstacleSpawn();

        void HandleCollisions();
        void HandleCollisionsPlayerObstacles();
    };
}