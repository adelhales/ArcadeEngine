// Copyright (c) 2025 Adel Hales

#include "TicTacToe.h"

using namespace TicTacToe;

Game::Game(EngineContext& context) :
    Scene(context),
    restartCooldown(RESTART_COOLDOWN_DURATION)
{
    InitGrid();
}

void Game::InitGrid()
{
    grid.cellSize = gConfig.windowSize / (float)GRID_SIZE;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            auto& cell = grid.cells[i][j];

            sf::Vector2f position = grid.cellSize.componentWiseMul({(float)j, (float)i});

            InitGridCell(cell, position);
        }
    }
}

void Game::InitGridCell(Cell& cell, sf::Vector2f position)
{
    cell.shape.setFillColor(CELL_COLOR);
    cell.shape.setOutlineColor(CELL_OUTLINE_COLOR);
    cell.shape.setOutlineThickness(-1);
    cell.shape.setSize(grid.cellSize);
    cell.shape.setPosition(position);

    cell.text.setCharacterSize(CELL_TEXT_SIZE);
    cell.text.setPosition(position + grid.cellSize / 2.f);
}

void Game::Start()
{
    StartGrid();
    StartTurn();
    StartRestartCooldown();
}

void Game::StartGrid()
{
    for (auto& row : grid.cells)
    {
        for (auto& cell : row)
        {
            StartGridCell(cell);
        }
    }
}

void Game::StartGridCell(Cell& cell)
{
    cell.symbol = ' ';
    cell.text.setString(cell.symbol);
}

void Game::StartTurn()
{
    isXTurn = true;
}

void Game::StartRestartCooldown()
{
    restartCooldown.Reset();
}

void Game::OnEvent(const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>() ||
        event.is<sf::Event::JoystickButtonPressed>())
    {
        EventPlayTurn();
        EventPlayTurn();
    }
}

void Game::Update()
{
    if (restartCooldown.IsOver())
    {
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::EventPlayTurn()
{
    if (restartCooldown.IsRunning())
    {
        return;
    }

    sf::Vector2i position = isXTurn ? GetCellPositionCursor() : GetCellPositionAI();

    if (IsCellPositionValid(position))
    {
        EventCellPlace(position);
        isXTurn = !isXTurn;
        EventCheckGrid();
    }
}

void Game::EventCellPlace(sf::Vector2i position)
{
    auto& cell = grid.cells[position.x][position.y];

    cell.symbol = isXTurn ? 'X' : 'O';

    cell.text.setFillColor(isXTurn ? CELL_SYMBOL_X_COLOR : CELL_SYMBOL_O_COLOR);
    cell.text.setString(cell.symbol);
    cell.text.setOrigin(cell.text.getLocalBounds().getCenter());
}

void Game::EventCheckGrid()
{
    if (IsWin('X'))
    {
        LOG_INFO("You Win!");
        restartCooldown.Start();
    }
    else if (IsWin('O'))
    {
        LOG_INFO("You Lose!");
        restartCooldown.Start();
    }
    else if (IsGridFull())
    {
        LOG_INFO("Draw!");
        restartCooldown.Start();
    }
}

sf::Vector2i Game::GetCellPositionCursor() const
{
    sf::Vector2i position(ctx.cursor.GetPosition().componentWiseDiv(grid.cellSize));
    return {position.y, position.x};
}

sf::Vector2i Game::GetCellPositionAI()
{
    std::vector<sf::Vector2i> positions = FindCellPositionsEmpty();

    return positions.empty() ? sf::Vector2i(-1, -1) :
           FindCellPositionWinning(positions, 'O').value_or(
           FindCellPositionWinning(positions, 'X').value_or(
           positions[ctx.random.Int(0, (int)positions.size() - 1)]));
}

std::vector<sf::Vector2i> Game::FindCellPositionsEmpty() const
{
    std::vector<sf::Vector2i> positions;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid.cells[i][j].symbol == ' ')
            {
                positions.emplace_back(i, j);
            }
        }
    }

    return positions;
}

std::optional<sf::Vector2i> Game::FindCellPositionWinning(std::span<sf::Vector2i> candidates, char symbol)
{
    for (sf::Vector2i position : candidates)
    {
        auto& cell = grid.cells[position.x][position.y];

        cell.symbol = symbol;
        bool win = IsWin(symbol);
        cell.symbol = ' ';

        if (win)
        {
            return position;
        }
    }

    return std::nullopt;
}

bool Game::IsCellPositionValid(sf::Vector2i position) const
{
    return position.x >= 0 && position.x < GRID_SIZE &&
           position.y >= 0 && position.y < GRID_SIZE &&
           grid.cells[position.x][position.y].symbol == ' ';
}

bool Game::IsWin(char symbol) const
{
    // Rows + Cols
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (std::ranges::all_of(grid.cells[i], [&](const Cell& cell) { return cell.symbol == symbol; }) ||
            std::ranges::all_of(grid.cells, [&](const auto& row) { return row[i].symbol == symbol; }))
        {
            return true;
        }
    }

    // Diagonals
    return std::ranges::all_of(std::views::iota(0, GRID_SIZE), [&](int i) { return grid.cells[i][i].symbol == symbol; }) ||
           std::ranges::all_of(std::views::iota(0, GRID_SIZE), [&](int i) { return grid.cells[i][GRID_SIZE - 1 - i].symbol == symbol; });
}

bool Game::IsGridFull() const
{
    return std::ranges::all_of(grid.cells, [](const auto& row) {
        return std::ranges::none_of(row, [](const Cell& cell) {
            return cell.symbol == ' ';
        });
    });
}

void Game::Render() const
{
    for (const auto& row : grid.cells)
    {
        for (const auto& cell : row)
        {
            ctx.renderer.Draw(cell.shape);
            ctx.renderer.Draw(cell.text);
        }
    }
}