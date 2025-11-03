// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/SnakeTypes.h"

namespace Snake
{
    class Game : public Scene
    {
    private:
        Player player;
        Bonus bonus;
        Map map;
        sf::Texture& tilesetTexture;
        sf::Sound bonusSound;

    public:
        Game(EngineContext&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitMap();
        void InitMapRect(sf::RectangleShape& rect, sf::Vector2i tile);
        void InitPlayer();
        void InitPlayerTextures();
        void InitBonus();

        void BindInputs();

        void StartPlayer();

        void UpdatePlayerDirection();
        void UpdatePlayerSmoothMovement();

        void EventPlayerMovement();
        void EventPlayerBodyMovement();
        void EventPlayerBodyPartMovement(Part& current, const Part& next);
        void EventPlayerBodySecondMovement();
        void EventPlayerHeadMovement();
        void EventPlayerTailMovement(Part& tail, const Part& next);
        void EventPlayerGrow();
        void EventCornerSpawn();
        void EventPlayerHeadChangeTexture(Part& head);
        void EventPlayerBodyChangeTexture(Part& body);
        void EventPlayerTailChangeTexture(Part& tail);
        void EventBonusSpawn();

        void HandleCollisions();
        void HandleCollisionsPlayerMap();
        void HandleCollisionsPlayerBody();
        void HandleCollisionsPlayerBonus();
        void ResolveCollisionPlayerBonus();

        sf::Vector2f GetPlayerHeadNextPosition() const;
        std::string GetCornerTextureKey() const;
    };
}