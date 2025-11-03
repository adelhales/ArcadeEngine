// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/BounceTypes.h"

namespace Bounce
{
    class Game : public Scene
    {
    private:
        Paddle paddle;
        std::vector<Ball> balls;
        Cooldown ballSpawnCooldown;
        Stats stats;
        sf::RectangleShape background;
        sf::Sound bounceSound;
        sf::Music music;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitPaddle();
        void InitStats();
        void InitBackground();
        void InitBounceSound();
        void InitMusic();

        void BindInputs();

        void StartPaddle();
        void StartStats();
        void StartMusic();

        void UpdatePaddle();
        void UpdateBalls();
        void UpdateBall(Ball& ball);

        void EventBallSpawn();
        void EventBallsMissed(int ballsMissed);

        void HandleCollisions();
        void HandleCollisionsPaddleBalls();
        void ResolveCollisionPaddleBall(Ball& ball);
        void HandleCollisionsBallsMap();
    };
}