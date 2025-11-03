// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/TowerDefenseTypes.h"

namespace TowerDefense
{
    class Game : public Scene
    {
    private:
        Stats stats;
        std::vector<Tower> towers;
        std::vector<Enemy> enemies;
        std::vector<Bullet> bullets;
        Castle castle;
        Wave wave;
        Map map;
        TowerPreview preview;
        UI ui;
        Cooldown towerSpawnCooldown;
        Cooldown waveSpawnCooldown;
        Cooldown enemySpawnCooldown;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitMap();
        void InitMapRect(sf::RectangleShape& rect, sf::Vector2i tile);
        void InitPath();
        void InitPathRect(sf::RectangleShape& rect);
        void InitCastle();

        void StartStats();
        void StartCastle();
        void StartPreview();
        void StartGui();

        void HandleEvent(const sf::Event::KeyPressed&);
        void HandleEvent(const sf::Event::MouseButtonPressed&);
        void HandleEvent(const sf::Event::JoystickButtonPressed&);
        void HandleEvent(const auto&) {}

        void UpdatePreview();
        void UpdateEnemies();
        void UpdateEnemyDirection(Enemy& enemy);
        void UpdateTowers();
        void UpdateBullets();
        void UpdateCastle();
        void UpdateHealth(Health& health, const sf::Shape& shape);
        void UpdateUI();

        void EventTowerPlacement();
        void EventTowerUpgrade(Tower& tower);
        void EventTowerPlace(sf::Vector2f position);
        void EventTowerShoot(Tower& tower);
        void EventBulletSpawn(Tower& tower, const Enemy& target);
        void EventWaveNew();
        void EventEnemySpawn();

        void HandleCollisions();
        void HandleCollisionsBullets();
        void HandleCollisionsBulletEnemies(Bullet& bullet);
        void ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy);
        void HandleCollisionsEnemies();
        void ResolveCollisionEnemyCastle(Enemy& enemy);

        tgui::ProgressBar::Ptr GenerateProgressBar(const tgui::String& title);
        Tower* TowerFind(sf::Vector2f position);
        sf::Vector2f WorldToTile(sf::Vector2f position) const;
        bool IsTileValid(sf::Vector2f position) const;
    };
}