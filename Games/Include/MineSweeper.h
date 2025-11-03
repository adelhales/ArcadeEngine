// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/MineSweeperTypes.h"

namespace MineSweeper
{
    class Game : public Scene
    {
    private:
        Grid grid;
        Stats stats;
        Cooldown restartCooldown;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;

    private:
        void InitGrid();
        void InitGridCell(Cell& cell, sf::FloatRect bounds);
        void InitStats();

        void StartGrid();
        void StartGridCell(Cell& cell);
        void StartStats();

        void HandleEvent(const sf::Event::MouseButtonPressed&);
        void HandleEvent(const sf::Event::JoystickButtonPressed&);
        void HandleEvent(const auto&) {}

        void EventMinesSpawn(sf::Vector2i safePosition);
        void EventMinesSpawnSetAdjacents();
        void EventCellRevealAsk();
        void EventCellReveal(sf::Vector2i position);
        void EventCellRevealAdjacentZeros(sf::Vector2i position);
        void EventGridReveal();
        void EventFlagToggleAsk();
        void EventFlagToggle(sf::Vector2i position);

        sf::Vector2i WorldToCell(sf::Vector2f position) const;
        sf::Vector2f GetGridOrigin() const;
        std::array<sf::Vector2i, 8> GetNeighborOffsets() const;
        bool IsCellPositionValid(sf::Vector2i position) const;
        bool IsWin() const;

        void RenderCell(const Cell& cell) const;
    };
}