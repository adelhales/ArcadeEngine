// Copyright (c) 2025 Adel Hales

#include "MineSweeper.h"

using namespace MineSweeper;

Game::Game(EngineContext& context) :
    Scene(context),
    restartCooldown(RESTART_COOLDOWN_DURATION)
{
    InitGrid();
    InitStats();
}

void Game::InitGrid()
{
    sf::Vector2f origin = GetGridOrigin();
    sf::Vector2f size(CELL_SIZE, CELL_SIZE);

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            auto& cell = grid.cells[i][j];

            sf::Vector2f offset(j * CELL_SIZE, i * CELL_SIZE);

            InitGridCell(cell, {origin + offset + size / 2.f, size});
        }
    }
}

void Game::InitGridCell(Cell& cell, sf::FloatRect bounds)
{
    cell.visual.background.setOutlineColor(CELL_OUTLINE_COLOR);
    cell.visual.background.setOutlineThickness(-1);
    cell.visual.background.setSize(bounds.size);
    cell.visual.background.setOrigin(cell.visual.background.getGeometricCenter());
    cell.visual.background.setPosition(bounds.position);

    cell.visual.mine.setFillColor(CELL_MINE_COLOR);
    cell.visual.mine.setRadius(CELL_SIZE / 4);
    cell.visual.mine.setOrigin(cell.visual.mine.getGeometricCenter());
    cell.visual.mine.setPosition(bounds.position);

    cell.visual.flag.setFillColor(CELL_FLAG_COLOR);
    cell.visual.flag.setRadius(CELL_SIZE / 4);
    cell.visual.flag.setPointCount(3);
    cell.visual.flag.setOrigin(cell.visual.flag.getGeometricCenter());
    cell.visual.flag.setPosition(bounds.position);

    cell.visual.adjacentCountText.setFillColor(CELL_TEXT_COLOR);
    cell.visual.adjacentCountText.setCharacterSize((int)CELL_SIZE - 10);
    cell.visual.adjacentCountText.setPosition(bounds.position);
}

void Game::InitStats()
{
    stats.mineCountText.setPosition({10, 10});
}

void Game::Start()
{
    StartGrid();
    StartStats();

    restartCooldown.Reset();
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
    cell.state = CellState::Hidden;
    cell.mined = false;
    cell.adjacentCount = 0;
    cell.visual.background.setFillColor(CELL_HIDDEN_COLOR);
}

void Game::StartStats()
{
    stats.firstClick = true;
    stats.flagCount = 0;
    stats.mineCountText.setString("Mines: " + std::to_string(STATS_MINE_COUNT));
}

void Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });

    stats.mineCountText.setString("Mines: " + std::to_string(STATS_MINE_COUNT - stats.flagCount));
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed& mouse)
{
    if (mouse.button == sf::Mouse::Button::Left)
    {
        EventCellRevealAsk();
    }
    else if (mouse.button == sf::Mouse::Button::Right)
    {
        EventFlagToggleAsk();
    }
}

void Game::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    if (auto button = Input::HardwareToLogical(joystick.button, joystick.joystickId))
    {
        if (*button == GamepadButton::South)
        {
            EventCellRevealAsk();
        }
        else if (*button == GamepadButton::West)
        {
            EventFlagToggleAsk();
        }
    }
}

void Game::Update()
{
    if (restartCooldown.IsOver())
    {
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::EventMinesSpawn(sf::Vector2i safePosition)
{
    int placed = 0;

    while (placed < STATS_MINE_COUNT)
    {
        sf::Vector2i position(ctx.random.Int(0, GRID_WIDTH - 1),
                              ctx.random.Int(0, GRID_HEIGHT - 1));

        auto& cell = grid.cells[position.y][position.x];

        if ((position == safePosition) || cell.mined)
        {
            continue;
        }

        cell.mined = true;
        placed++;
    }

    EventMinesSpawnSetAdjacents();
}

void Game::EventMinesSpawnSetAdjacents()
{
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            auto& cell = grid.cells[i][j];

            cell.adjacentCount = cell.mined ? -1 :
                (int)std::ranges::count_if(GetNeighborOffsets(), [&](sf::Vector2i offset) {
                    int neighborX = j + offset.x;
                    int neighborY = i + offset.y;
                    return neighborX >= 0 && neighborX < GRID_WIDTH && 
                           neighborY >= 0 && neighborY < GRID_HEIGHT && 
                           grid.cells[neighborY][neighborX].mined;
                });

            if (cell.adjacentCount > 0)
            {
                cell.visual.adjacentCountText.setString(std::to_string(cell.adjacentCount));
                sf::Vector2f center = cell.visual.adjacentCountText.getLocalBounds().getCenter();
                cell.visual.adjacentCountText.setOrigin(center);
            }
        }
    }
}

void Game::EventCellRevealAsk()
{
    if (restartCooldown.IsRunning())
    {
        return;
    }

    sf::Vector2i position = WorldToCell(ctx.cursor.GetPosition());

    if (!IsCellPositionValid(position))
    {
        return;
    }

    if (stats.firstClick)
    {
        EventMinesSpawn(position);
        stats.firstClick = false;
    }

    auto& cell = grid.cells[position.y][position.x];
    
    if (cell.state == CellState::Hidden)
    {
        EventCellReveal(position);
    }
}

void Game::EventCellReveal(sf::Vector2i position)
{
    auto& cell = grid.cells[position.y][position.x];
    
    if (cell.state == CellState::Revealed || cell.state == CellState::Flagged)
    {
        return;
    }

    cell.state = CellState::Revealed;
    cell.visual.background.setFillColor(CELL_REVEALED_COLOR);

    if (cell.mined)
    {
        LOG_INFO("You Lose!");
        EventGridReveal();
        return;
    }

    if (cell.adjacentCount == 0)
    {
        EventCellRevealAdjacentZeros(position);
    }

    if (IsWin())
    {
        LOG_INFO("You Win!");
        EventGridReveal();
    }
}

void Game::EventCellRevealAdjacentZeros(sf::Vector2i position)
{
    for (sf::Vector2i offset : GetNeighborOffsets())
    {
        sf::Vector2i neighborPosition = position + offset;

        if (!IsCellPositionValid(neighborPosition))
        {
            continue;
        }

        auto& cell = grid.cells[neighborPosition.y][neighborPosition.x];

        if ((cell.state != CellState::Hidden && cell.state != CellState::Flagged) || cell.mined)
        {
            continue;
        }

        if (cell.state == CellState::Flagged)
        {
            stats.flagCount--;
        }

        cell.state = CellState::Revealed;
        cell.visual.background.setFillColor(CELL_REVEALED_COLOR);

        if (cell.adjacentCount == 0)
        {
            EventCellRevealAdjacentZeros(neighborPosition);
        }
    }
}

void Game::EventGridReveal()
{
    for (auto& row : grid.cells)
    {
        for (auto& cell : row)
        {
            if (cell.mined)
            {
                cell.state = CellState::Revealed;
                cell.visual.background.setFillColor(CELL_REVEALED_COLOR);
            }
        }
    }

    restartCooldown.Start();
}

void Game::EventFlagToggleAsk()
{
    if (restartCooldown.IsRunning())
    {
        return;
    }

    sf::Vector2i position = WorldToCell(ctx.cursor.GetPosition());

    if (IsCellPositionValid(position))
    {
        EventFlagToggle(position);
    }
}

void Game::EventFlagToggle(sf::Vector2i position)
{
    auto& cell = grid.cells[position.y][position.x];

    if (cell.state == CellState::Revealed)
    {
        return;
    }

    if (cell.state == CellState::Hidden)
    {
        if (stats.flagCount < STATS_MINE_COUNT)
        {
            cell.state = CellState::Flagged;
            stats.flagCount++;
            cell.visual.background.setFillColor(CELL_REVEALED_COLOR);
        }
    }
    else if (cell.state == CellState::Flagged)
    {
        cell.state = CellState::Hidden;
        stats.flagCount--;
        cell.visual.background.setFillColor(CELL_HIDDEN_COLOR);
    }
}

sf::Vector2i Game::WorldToCell(sf::Vector2f position) const
{
    sf::Vector2f offset = position - GetGridOrigin();
    return {int(std::floor(offset.x / CELL_SIZE)), int(std::floor(offset.y / CELL_SIZE))};
}

sf::Vector2f Game::GetGridOrigin() const
{
    return (gConfig.windowSize - sf::Vector2f(GRID_WIDTH, GRID_HEIGHT) * CELL_SIZE) / 2.f;
}

std::array<sf::Vector2i, 8> Game::GetNeighborOffsets() const
{
    return std::to_array<sf::Vector2i>(
    {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    });
}

bool Game::IsCellPositionValid(sf::Vector2i position) const
{
    return position.x >= 0 && position.x < GRID_WIDTH &&
           position.y >= 0 && position.y < GRID_HEIGHT;
}

bool Game::IsWin() const
{
    int revealed = 0;

    for (const auto& row : grid.cells)
    {
        revealed += (int)std::ranges::count_if(row, [](const Cell& cell) {
            return !cell.mined && cell.state == CellState::Revealed;
        });
    }

    return revealed == (GRID_WIDTH * GRID_HEIGHT - STATS_MINE_COUNT);
}

void Game::Render() const
{
    for (const auto& row : grid.cells)
    {
        for (const auto& cell : row)
        {
            RenderCell(cell);
        }
    }

    ctx.renderer.Draw(stats.mineCountText);
}

void Game::RenderCell(const Cell& cell) const
{
    ctx.renderer.Draw(cell.visual.background);

    if (cell.state == CellState::Flagged)
    {
        ctx.renderer.Draw(cell.visual.flag);
    }
    else if (cell.state == CellState::Revealed)
    {
        if (cell.mined)
        {
            ctx.renderer.Draw(cell.visual.mine);
        }
        else if (cell.adjacentCount > 0)
        {
            ctx.renderer.Draw(cell.visual.adjacentCountText);
        }
    }
}