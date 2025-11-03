// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/TicTacToeTypes.h"

#include <span>

namespace TicTacToe
{
    class Game : public Scene
    {
    private:
        Grid grid;
        bool isXTurn;
        Cooldown restartCooldown;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;

    private:
        void InitGrid();
        void InitGridCell(Cell& cell, sf::Vector2f position);

        void StartGrid();
        void StartGridCell(Cell& cell);
        void StartTurn();
        void StartRestartCooldown();

        void EventPlayTurn();
        void EventCellPlace(sf::Vector2i position);
        void EventCheckGrid();

        sf::Vector2i GetCellPositionCursor() const;
        sf::Vector2i GetCellPositionAI();
        std::vector<sf::Vector2i> FindCellPositionsEmpty() const;
        std::optional<sf::Vector2i> FindCellPositionWinning(std::span<sf::Vector2i> candidates, char symbol);
        bool IsCellPositionValid(sf::Vector2i position) const;
        bool IsWin(char symbol) const;
        bool IsGridFull() const;
    };
}