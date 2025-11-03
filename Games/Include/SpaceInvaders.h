// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/SpaceInvadersTypes.h"

namespace SpaceInvaders
{
    class Game : public Scene
    {
    private:
        Player player;
        Wave wave;
        std::vector<Bunker> bunkers;
        std::vector<Bullet> bullets;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;

    private:
        void InitPlayer();
        void InitWave();

        void BindInputs();

        void StartPlayer();
        void StartWave();
        void StartEnemy(Enemy& enemy, int i, int j);
        void StartBunkers();
        void StartBunker(Bunker& bunker, int i);
        void StartBunkerPart(BunkerPart& part, sf::FloatRect bounds);

        void UpdatePlayer();
        void UpdateBullets();

        void EventPlayerShoot();
        void EventEnemiesMovement();
        void EventEnemyShoot();
        void EventEnemyDead();

        void HandleCollisions();
        void HandleCollisionsEnemiesMap();
        void HandleCollisionsEnemiesBunker();
        void HandleCollisionsBullets();
        void HandleCollisionsBulletPlayer(Bullet& bullet);
        void HandleCollisionsBulletEnemies(Bullet& bullet);
        void ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy);
        void HandleCollisionsBulletBunkers(Bullet& bullet);
        void ResolveCollisionBulletBunker(Bullet& bullet, Bunker& bunker);
        void ResolveCollisionBulletBunkerPart(Bullet& bullet, BunkerPart& part);
    };
}