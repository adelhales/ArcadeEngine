// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/BreakoutTypes.h"

namespace Breakout
{
    class Game : public Scene
    {
    private:
        Player player;
        std::vector<Ball> balls;
        std::vector<Brick> bricks;
        std::vector<Bonus> bonuses;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitPlayer();

        void StartPlayer();
        void StartBricks();
        void StartBrick(Brick& brick, sf::Vector2i tile);

        void UpdatePlayer();
        void UpdateBalls();
        void UpdateBall(Ball& ball);
        void UpdateBonuses();
        void UpdateBonusesCleanup();

        void EventBallSpawn();
        void EventBallReset(Ball& ball);
        void EventBonusEnable();
        void EventBonusDisable(const Bonus& bonus);

        void HandleCollisions();
        void HandleCollisionsBallsMap();
        void HandleCollisionsBallMap(Ball& ball);
        void ResolveCollisionBallPlayer(Ball& ball);
        void ResolveCollisionBallDown(Ball& ball);
        void HandleCollisionsBallBricks(Ball& ball);
        void ResolveCollisionBallBrick(Ball& ball, Brick& brick);
        void ResolveCollisionBallBrickBounce(Ball& ball, Brick& brick);

        std::vector<BonusType> GetBonusTypesAllowed() const;
        bool IsBonusCompatible(BonusType a, BonusType b) const;
        bool IsBallNearPlayer(const Ball& ball) const;
    };
}