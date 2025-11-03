// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/AdventureTypes.h"

#include "Graphics/TileMap.h"

namespace Adventure
{
    class Game : public Scene
    {
    private:
        Player player;
        Camera camera;
        std::vector<Bullet> bullets;
        std::vector<Enemy> enemies;
        TileMap map;
        Cooldown enemySpawnCooldown;
        sf::Sound bonusSound;
        sf::RectangleShape background;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitPlayer();
        void InitCamera();
        void InitSounds();
        void InitBackground();

        void BindInputs();
        
        void StartMap();
        void StartPlayer();
        void StartCamera();

        void UpdatePlayer();
        void UpdatePlayerDirection();
        void UpdatePlayerHorizontalMovement();
        void UpdatePlayerClimbing();
        void UpdatePlayerGravity();
        void UpdatePlayerVerticalMovement();
        void UpdatePlayerAnimation();
        void UpdateCamera();
        void UpdateEnemies();
        void UpdateBullets();

        void EventPlayerShoot();
        void EventPlayerForceField();
        void EventPlayerJump();
        void EventPlayerBonus();
        void EventEnemySpawn();
        void EventEnemyKnockback(Enemy& enemy, sf::Vector2f velocity, int damage);

        void HandleCollisions();
        void HandleCollisionsPlayer();
        void HandleCollisionsPlayerUp();
        void HandleCollisionsPlayerDown();
        void HandleCollisionsPlayerLeftRight();
        void HandleCollisionsPlayerBonus();
        void HandleCollisionsPlayerEnemies();
        void ResolveCollisionPlayerEnemy(Enemy& enemy);
        void HandleCollisionsBullets();
        void HandleCollisionsBulletEnemies(Bullet& bullet);
        void ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy);
        void HandleCollisionsBulletMap(Bullet& bullet);
        void HandleCollisionsEnemies();
        void ResolveCollisionEnemyEnemy(Enemy& left, Enemy& right);

        sf::Vector2u GetPlayerCurrentTile() const;
        sf::Vector2f GetPlayerShootDirection() const;
        bool IsTileSolid(sf::Vector2u position) const;
        bool IsTileLadder(sf::Vector2u position) const;
        bool IsTileBonus(sf::Vector2u position) const;
        bool IsTileEmpty(sf::Vector2u position) const;
    };
}