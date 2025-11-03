// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/MineStormTypes.h"

namespace MineStorm
{
    class Game : public Scene
    {
    private:
        Player player;
        std::vector<Enemy> enemies;
        std::vector<Bullet> bullets;
        std::vector<Explosion> explosions;
        Wave wave;
        sf::RectangleShape background;
        sf::RectangleShape foreground;
        sf::Texture& explosionTexture;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitPlayer();
        void InitBackground();

        void BindInputs();

        void StartPlayer();

        void UpdatePlayer();
        void UpdatePlayerRotation();
        void UpdatePlayerAccelerate();
        void UpdatePlayerDecelerate();
        void UpdateEnemies();
        void UpdateEnemy(Enemy& enemy);
        void UpdateBullets();
        void UpdateExplosions();

        void EventPlayerShoot();
        void EventPlayerShipReset();
        void EventWaveNew();
        void EventEnemySpawn();
        void EventEnemyShoot(const Enemy& enemy);
        void EventEnemyDead(const Enemy& enemy);
        void EventExplosion(const sf::Shape& shape);

        void HandleCollisions();
        void HandleCollisionsPlayerMap();
        void HandleCollisionsPlayerEnemies();
        void ResolveCollisionPlayerEnemy();
        void HandleCollisionsEnemiesMap();
        void HandleCollisionsShapeMap(sf::Shape& shape);
        void HandleCollisionsBullets();
        void ResolveCollisionBulletPlayer(Bullet& bullet);
        void HandleCollisionsBulletEnemies(Bullet& bullet);
        void ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy);

        sf::CircleShape GenerateSpawnPoint() const;
    };
}