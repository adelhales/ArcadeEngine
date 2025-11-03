// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/PongTypes.h"

namespace Pong
{
    class Game : public Scene
    {
    private:
        Player player1;
        Player player2;
        Ball ball;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;

    private:
        void InitPlayerLeft(Player& player);
        void InitPlayerRight(Player& player);
        void InitPlayerBase(Player& player);
        void InitBall();

        void BindInputs();

        void StartPlayerLeft(Player& player);
        void StartPlayerRight(Player& player);
        void StartPlayerBase(Player& player);

        void UpdatePlayer(Player& player, Action up, Action down);
        void UpdatePlayerAI(Player& player);
        void UpdateBall();

        void EventPlayerWin(Player& player);
        void EventBallReset();

        void HandleCollisions();
        void HandleCollisionsPlayersBall();
        void ResolveCollisionPlayerBall(const Player& player);
        void HandleCollisionsBallMap();

        void RenderMap() const;
    };
}