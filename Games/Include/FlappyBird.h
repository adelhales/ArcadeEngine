// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/FlappyBirdTypes.h"

namespace FlappyBird
{
    class Game : public Scene
    {
    private:
        Generation generation;
        std::vector<Obstacle> obstacles;
        sf::Clock clock;
        sf::RectangleShape background;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitGeneration();
        void InitBirds();
        void InitBird(Bird& bird);
        void InitStats();
        void InitBackground();

        void StartGeneration();
        void StartStats();
        void StartBirds();
        void StartBird(Bird& bird);

        void UpdateGeneration();
        void UpdateBird(Bird& bird);
        void UpdateBirdTargetObstacle(Bird& bird);
        void UpdateBirdMove(Bird& bird);
        void UpdateBirdRotation(Bird& bird);
        void UpdateObstacles();
        void UpdateObstacle(Obstacle& obstacle);

        void EventRestartBird(Bird& bird);
        void EventObstacleSpawn();
        void EventGenerationNext();
        void EventGenerationNextStats();

        void HandleCollisions();
        void HandleCollisionsBirds();
        void ResolveCollisionBird(Bird& bird);
        void HandleCollisionsObstacle();
        void ResolveCollisionObstacle();

        float GetSigmoid(float value) const;
        bool ShouldBirdJump(const Bird& bird) const;
        bool ShouldBirdDie(const Bird& bird) const;
        bool ShouldObstacleSpawn() const;
    };
}