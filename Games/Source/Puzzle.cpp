// Copyright (c) 2025 Adel Hales

#include "Puzzle.h"

using namespace Puzzle;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitGrid();
}

void Game::InitGrid()
{
    InitCells();
    InitBackground();
}

void Game::InitCells()
{
    for (auto& row : grid.cells)
    {
        for (auto& cell : row)
        {
            InitCell(cell);
        }
    }
}

void Game::InitCell(Cell& cell)
{
    cell.shape.setOutlineColor(sf::Color::Black);
    cell.shape.setOutlineThickness(-1);
    cell.shape.setSize(gConfig.windowSize / (float)GRID_SIZE);

    cell.text.setCharacterSize(CELL_TEXT_SIZE);
}

void Game::InitBackground()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            auto& cell = grid.background[i][j];

            cell.setFillColor(CELL_COLOR_EMPTY);
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(-1);
            cell.setSize(gConfig.windowSize / (float)GRID_SIZE);
            cell.setPosition(cell.getSize().componentWiseMul({(float)j, (float)i}));
        }
    }
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);

    BindInputs();

    StartGrid();
    StartStats();

    for (int i = 0; i < 2; i++)
    {
        EventCellNew();
    }
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveLeft,  Input::Axis{sf::Joystick::Axis::X, -0.5f});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::X,  0.5f});
    ctx.input.Bind(MoveUp,    Input::Axis{sf::Joystick::Axis::Y, -0.5f});
    ctx.input.Bind(MoveDown,  Input::Axis{sf::Joystick::Axis::Y,  0.5f});
}

void Game::StartGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            auto& cell = grid.cells[i][j];

            StartCell(cell, {j, i});
        }
    }
}

void Game::StartCell(Cell& cell, sf::Vector2i tile)
{
    cell.shape.setPosition((sf::Vector2f(tile).componentWiseMul(cell.shape.getSize())));
    cell.text.setPosition(cell.shape.getPosition() + cell.shape.getSize() / 2.f);

    cell.value = 0;
    cell.animating = false;
}

void Game::StartStats()
{
    stats.previousPressed = false;
}

void Game::OnEvent(const sf::Event& event)
{
    if (!event.is<sf::Event::KeyPressed>() || IsAnimating())
    {
        return;
    }

    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        std::optional<int> turns;

        switch (key->scancode)
        {
            case sf::Keyboard::Scan::Left:  turns = 0; break;
            case sf::Keyboard::Scan::Down:  turns = 1; break;
            case sf::Keyboard::Scan::Right: turns = 2; break;
            case sf::Keyboard::Scan::Up:    turns = 3; break;
            default: break;
        }

        EventCellAction(turns.value_or(-1));
    }
}

void Game::Update()
{
    UpdateActions();

    if (IsAnimating())
    {
        UpdateAnimation();

        if (!IsAnimating())
        {
            UpdateAnimationFinished();
            EventCellNew();

            if (IsWin())
            {
                LOG_INFO("You Win!");
                ctx.scenes.RestartCurrentScene();
            }
            else if (IsLose())
            {
                LOG_INFO("You Lose!");
                ctx.scenes.RestartCurrentScene();
            }
        }
    }
}

void Game::UpdateActions()
{
    if (IsAnimating())
    {
        return;
    }

    bool left  = ctx.input.Pressed(MoveLeft);
    bool down  = ctx.input.Pressed(MoveDown);
    bool right = ctx.input.Pressed(MoveRight);
    bool up    = ctx.input.Pressed(MoveUp);

    bool currentPressed = left || down || right || up;

    if (currentPressed && !stats.previousPressed)
    {
        int turns = left ? 0 : down ? 1 : right ? 2 : up ? 3 : -1;
        EventCellAction(turns);
    }

    stats.previousPressed = currentPressed;
}

void Game::UpdateAnimation()
{
    float progress = stats.animationProgress / ANIMATION_DURATION;
    
    stats.animationProgress += ctx.time.GetDeltaTime();

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            auto& cell = grid.cells[i][j];

            if (cell.animating)
            {
                UpdateAnimationCell(cell, progress);
            }
        }
    }
}

void Game::UpdateAnimationCell(Cell& cell, float progress)
{
    sf::Vector2f position = Lerp(cell.startPosition, cell.endPosition, progress);

    cell.shape.setPosition(position);
    cell.text.setPosition(position + cell.shape.getSize() / 2.f);

    if (stats.animationProgress >= ANIMATION_DURATION)
    {
        cell.animating = false;
    }
}

void Game::UpdateAnimationFinished()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            auto& cell = grid.cells[i][j];

            UpdateAnimationFinishedCell(cell);
        }
    }
}

void Game::UpdateAnimationFinishedCell(Cell& cell)
{
    cell.value = cell.targetValue;

    cell.shape.setPosition(cell.startPosition);
    cell.text.setPosition(cell.startPosition + cell.shape.getSize() / 2.f);

    if (cell.value > 0)
    {
        EventCellChanged(cell);
    }
}

void Game::EventCellNew()
{
    std::vector<sf::Vector2i> emptyCells = GetEmptyCells();

    if (emptyCells.empty())
    {
        return;
    }

    auto [i, j] = emptyCells[ctx.random.Int(0, (int)emptyCells.size() - 1)];

    grid.cells[i][j].value = (int)std::pow(2, ctx.random.Int(1, 2));
    grid.cells[i][j].animating = false;

    EventCellChanged(grid.cells[i][j]);
}

void Game::EventCellChanged(Cell& cell)
{
    cell.shape.setFillColor(GetCellColor(cell));

    cell.text.setFillColor(GetTextColor(cell));
    cell.text.setString(std::to_string(cell.value));
    cell.text.setOrigin(cell.text.getLocalBounds().getCenter());
}

void Game::EventCellAction(int turns)
{
    if (turns > 0)
    {
        EventCellsActionRotate(turns);
        EventCellsActionMoveLeft();
        EventCellsActionRotate(4 - turns);
    }
    else if (turns == 0)
    {
        EventCellsActionMoveLeft();
    }
}

void Game::EventCellsActionRotate(int turns)
{
    for (int t = 0; t < turns; t++)
    {
        decltype(grid.cells) temp;

        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                temp[j][GRID_SIZE - 1 - i] = grid.cells[i][j];
            }
        }

        grid.cells = temp;
    }
}

void Game::EventCellsActionMoveLeft()
{
    EventCellsActionMoveLeftPrepare();

    std::array<std::array<bool, GRID_SIZE>, GRID_SIZE> merged = {};

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 1; j < GRID_SIZE; j++)
        {
            if (grid.cells[i][j].targetValue != 0)
            {
                EventCellsActionMoveLeftApply(merged, i, j);
            }
        }
    }
}

void Game::EventCellsActionMoveLeftPrepare()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid.cells[i][j].startPosition = grid.cells[i][j].shape.getPosition();
            grid.cells[i][j].targetValue = grid.cells[i][j].value;
        }
    }
}

void Game::EventCellsActionMoveLeftApply(std::array<std::array<bool, GRID_SIZE>, GRID_SIZE>& merged, int i, int j)
{
    int k = GetCellLeftDestination(i, j);

    if (k > 0 && grid.cells[i][j].targetValue == grid.cells[i][k - 1].targetValue && !merged[i][k - 1])
    {
        merged[i][k - 1] = true;

        EventCellsMerge(grid.cells[i][j], grid.cells[i][k - 1]);
        grid.cells[i][k - 1].targetValue *= 2;
        grid.cells[i][j].targetValue = 0;
    }
    else if (grid.cells[i][k].targetValue == 0)
    {
        EventCellsMerge(grid.cells[i][j], grid.cells[i][k]);
        grid.cells[i][k].targetValue = grid.cells[i][j].targetValue;
        grid.cells[i][j].targetValue = 0;
    }
}

void Game::EventCellsMerge(Cell& fromCell, Cell& toCell)
{
    fromCell.endPosition = toCell.startPosition;
    fromCell.animating = true;
    stats.animationProgress = 0;
}

std::vector<sf::Vector2i> Game::GetEmptyCells() const
{
    std::vector<sf::Vector2i> emptyCells;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid.cells[i][j].value == 0)
            {
                emptyCells.emplace_back(i, j);
            }
        }
    }

    return emptyCells;
}

int Game::GetCellLeftDestination(int i, int j) const
{
    int k = j;

    while (k > 0 && grid.cells[i][k - 1].targetValue == 0)
    {
        k--;
    }

    return k;
}

sf::Color Game::GetCellColor(const Cell& cell) const
{
    return CELLS_COLORS[(int)std::log2(cell.value)];
}

sf::Color Game::GetTextColor(const Cell& cell) const
{
    return (cell.value > 4) ? CELLS_TEXT_COLORS[0] : CELLS_TEXT_COLORS[1];
}

bool Game::IsAnimating() const
{
    for (const auto& row : grid.cells)
    {
        if (std::ranges::any_of(row, [](const Cell& cell) { return cell.animating; }))
        {
            return true;
        }
    }

    return false;
}

bool Game::IsWin() const
{
    for (const auto& row : grid.cells)
    {
        if (std::ranges::any_of(row, [](const Cell& cell) { return cell.value == STATS_TARGET_VALUE; }))
        {
            return true;
        }
    }

    return false;
}

bool Game::IsLose() const
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid.cells[i][j].value == 0 || 
                (j < GRID_SIZE - 1 && grid.cells[i][j].value == grid.cells[i][j + 1].value) ||
                (i < GRID_SIZE - 1 && grid.cells[i][j].value == grid.cells[i + 1][j].value))
            {
                return false;
            }
        }
    }

    return true;
}

void Game::Render() const
{
    for (auto& row : grid.background)
    {
        for (auto& cell : row)
        {
            ctx.renderer.Draw(cell);
        }
    }

    for (auto& row : grid.cells)
    {
        for (auto& cell : row)
        {
            if (cell.value > 0)
            {
                ctx.renderer.Draw(cell.shape);
                ctx.renderer.Draw(cell.text);
            }
        }
    }
}