// Copyright (c) 2025 Adel Hales

#include "Scene/SceneFactory.h"

#include "Adventure.h"
#include "Bounce.h"
#include "Breakout.h"
#include "Clicker.h"
#include "FlappyBird.h"
#include "LevelEditor.h"
#include "MemoryCard.h"
#include "Menu.h"
#include "MineStorm.h"
#include "MineSweeper.h"
#include "Pong.h"
#include "Puzzle.h"
#include "Runner.h"
#include "Snake.h"
#include "SpaceInvaders.h"
#include "Tetris.h"
#include "TicTacToe.h"
#include "TowerDefense.h"

namespace SceneFactory
{
    Scenes CreateScenes(EngineContext& context)
    {
        Scenes scenes;

        scenes.emplace("Bounce",         std::make_unique<Bounce::Game>(context));
        scenes.emplace("Menu",           std::make_unique<Menu::Game>(context));
        scenes.emplace("Clicker",        std::make_unique<Clicker::Game>(context));
        scenes.emplace("Memory Card",    std::make_unique<MemoryCard::Game>(context));
        scenes.emplace("Tic Tac Toe",    std::make_unique<TicTacToe::Game>(context));
        scenes.emplace("Mine Sweeper",   std::make_unique<MineSweeper::Game>(context));
        scenes.emplace("Runner",         std::make_unique<Runner::Game>(context));
        scenes.emplace("Tetris",         std::make_unique<Tetris::Game>(context));
        scenes.emplace("Pong",           std::make_unique<Pong::Game>(context));
        scenes.emplace("Breakout",       std::make_unique<Breakout::Game>(context));
        scenes.emplace("Flappy Bird",    std::make_unique<FlappyBird::Game>(context));
        scenes.emplace("Space Invaders", std::make_unique<SpaceInvaders::Game>(context));
        scenes.emplace("Puzzle",         std::make_unique<Puzzle::Game>(context));
        scenes.emplace("Snake",          std::make_unique<Snake::Game>(context));
        scenes.emplace("Mine Storm",     std::make_unique<MineStorm::Game>(context));
        scenes.emplace("Tower Defense",  std::make_unique<TowerDefense::Game>(context));
        scenes.emplace("Level Editor",   std::make_unique<LevelEditor::Game>(context));
        scenes.emplace("Adventure",      std::make_unique<Adventure::Game>(context));

        return scenes;
    }
}