// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/TowerDefenseConfig.h"

namespace TowerDefense
{
    struct UI
    {
        tgui::Panel::Ptr container;
        tgui::VerticalLayout::Ptr layout;
        tgui::ProgressBar::Ptr enemyBar;
        tgui::ProgressBar::Ptr towerBar;
        tgui::ProgressBar::Ptr waveBar;
        tgui::Label::Ptr waveLabel;
        tgui::Label::Ptr statsLabel;
    };

    struct Health
    {
        sf::RectangleShape barBackground;
        sf::RectangleShape bar;
        int maxPoints;
        int points;
    };

    struct Map
    {
        sf::Vector2i size;
        sf::Vector2f tileSize;
        std::vector<sf::RectangleShape> grid;
        std::vector<sf::RectangleShape> path;
    };

    struct Stats
    {
        int money;
        int level;
    };

    struct Enemy
    {
        sf::RectangleShape shape;
        Health health;
        sf::Vector2f direction;
        float speed;
        int damage;
        int pathIndex;
        int level;
    };

    struct Wave
    {
        std::vector<int> enemies;
    };

    struct Bullet
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
        int damage;
        bool alive;
    };

    struct Tower
    {
        sf::CircleShape shape;
        sf::CircleShape area;
        Cooldown shootCooldown;
        int level;
    };

    struct TowerPreview
    {
        sf::CircleShape shape;
        sf::CircleShape area;
        bool enabled;
    };

    struct Castle
    {
        sf::RectangleShape shape;
        Health health;
    };
}