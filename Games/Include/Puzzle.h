// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/PuzzleTypes.h"

namespace Puzzle
{
    class Game : public Scene
    {
    private:
        Grid grid;
        Stats stats;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;

    private:
        void InitGrid();
        void InitCells();
        void InitCell(Cell& cell);
        void InitBackground();

        void BindInputs();

        void StartGrid();
        void StartCell(Cell& cell, sf::Vector2i tile);
        void StartStats();

        void UpdateActions();
        void UpdateAnimation();
        void UpdateAnimationCell(Cell& cell, float progress);
        void UpdateAnimationFinished();
        void UpdateAnimationFinishedCell(Cell& cell);

        void EventCellNew();
        void EventCellChanged(Cell& cell);
        void EventCellAction(int turns);
        void EventCellsActionRotate(int turns);
        void EventCellsActionMoveLeft();
        void EventCellsActionMoveLeftPrepare();
        void EventCellsActionMoveLeftApply(std::array<std::array<bool, GRID_SIZE>, GRID_SIZE>& merged, int i, int j);
        void EventCellsMerge(Cell& from, Cell& to);

        std::vector<sf::Vector2i> GetEmptyCells() const;
        int GetCellLeftDestination(int i, int j) const;
        sf::Color GetCellColor(const Cell& cell) const;
        sf::Color GetTextColor(const Cell& cell) const;
        bool IsAnimating() const;
        bool IsWin() const;
        bool IsLose() const;
    };
}