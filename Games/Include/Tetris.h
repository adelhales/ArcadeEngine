// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/TetrisTypes.h"

namespace Tetris
{
    class Game : public Scene
    {
    private:
        Board board;
        Piece current;
        Piece next;
        Stats stats;
        Cooldown actionCooldown;
        Cooldown fallCooldown;
        Cooldown softDropCooldown;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitStats();

        void BindInputs();

        void StartGrid();
        void StartNextPiece();
        void StartStats();

        void EventPieceSpawn();
        void EventPieceRotate();
        void EventPieceMove(sf::Vector2i offset);
        void EventPieceFall();
        void EventPieceDropHard();
        void EventPieceMerge();
        void EventLinesClear();

        Piece GenerateRandomPiece() const;
        sf::Vector2i GetPointRotated(int type, int index, int rotation) const;
        sf::Vector2f GetBoardOrigin() const;
        bool IsPieceValid(const Piece& piece) const;

        void RenderBoard() const;
        void RenderPiece(const Piece& piece, sf::Vector2f origin, bool world) const;
    };
}