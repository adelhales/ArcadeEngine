// Copyright (c) 2025 Adel Hales

#include "Tetris.h"

using namespace Tetris;

Game::Game(EngineContext& context) :
    Scene(context),
    actionCooldown(ACTION_COOLDOWN_DURATION),
    fallCooldown(PIECE_FALL_COOLDOWN_DURATION),
    softDropCooldown(SOFT_DROP_COOLDOWN_DURATION)
{
    InitStats();
}

void Game::InitStats()
{
    stats.scoreText.setPosition({10, 10});
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);
    
    BindInputs();
    
    StartGrid();
    StartNextPiece();
    StartStats();

    EventPieceSpawn();

    actionCooldown.Restart();
    fallCooldown.Restart();
    softDropCooldown.Restart();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::Right});
    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::X, 0.5f});

    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::Left});
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::X, -0.5f});

    ctx.input.Bind(Rotate, Input::Keyboard{sf::Keyboard::Scan::Up});
    ctx.input.Bind(Rotate, Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(Rotate, Input::Gamepad{GamepadButton::North});

    ctx.input.Bind(SoftDrop, Input::Keyboard{sf::Keyboard::Scan::Down});
    ctx.input.Bind(SoftDrop, Input::Keyboard{sf::Keyboard::Scan::S});
    ctx.input.Bind(SoftDrop, Input::Axis{sf::Joystick::Axis::Y, 0.5f});
}

void Game::StartGrid()
{
    board = {};
}

void Game::StartNextPiece()
{
    next = GenerateRandomPiece();
}

void Game::StartStats()
{
    stats.score = 0;
    stats.scoreText.setString("Score: 0");
}

void Game::OnEvent(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode == sf::Keyboard::Scan::Space)
        {
            EventPieceDropHard();
        }
    }

    if (auto joystick = event.getIf<sf::Event::JoystickButtonPressed>())
    {
        if (Input::HardwareToLogical(joystick->button, joystick->joystickId) == GamepadButton::South)
        {
            EventPieceDropHard();
        }
    }
}

void Game::Update()
{
    if (ctx.input.Pressed(Rotate) && actionCooldown.IsOver())
    {
        EventPieceRotate();
        actionCooldown.Restart();
    }

    if (ctx.input.Pressed(MoveRight) && actionCooldown.IsOver())
    {
        EventPieceMove({1, 0});
        actionCooldown.Restart();
    }
    else if (ctx.input.Pressed(MoveLeft) && actionCooldown.IsOver())
    {
        EventPieceMove({-1, 0});
        actionCooldown.Restart();
    }

    if (ctx.input.Pressed(SoftDrop) && softDropCooldown.IsOver())
    {
        EventPieceFall();
        softDropCooldown.Restart();
        fallCooldown.Restart();
    }
    else if (fallCooldown.IsOver())
    {
        EventPieceFall();
        fallCooldown.Restart();
    }
}

void Game::EventPieceSpawn()
{
    current = next;
    next = GenerateRandomPiece();
}

void Game::EventPieceRotate()
{
    Piece rotated = current;
    rotated.rotation = (rotated.rotation + 1) % 4;

    if (IsPieceValid(rotated))
    {
        current = rotated;
    }
}

void Game::EventPieceMove(sf::Vector2i offset)
{
    Piece moved = current;
    moved.position += offset;

    if (IsPieceValid(moved))
    {
        current = moved;
    }
}

void Game::EventPieceFall()
{
    Piece moved = current;
    moved.position.y++;

    if (IsPieceValid(moved))
    {
        current = moved;
    }
    else
    {
        EventPieceMerge();
    }
}

void Game::EventPieceDropHard()
{
    while (IsPieceValid(current))
    {
        current.position.y++;
    }
    current.position.y--;

    EventPieceMerge();
}

void Game::EventPieceMerge()
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        sf::Vector2i block = GetPointRotated(current.type, i, current.rotation) + current.position;

        if (block.y >= 0 && block.y < GRID_HEIGHT &&
            block.x >= 0 && block.x < GRID_WIDTH)
        {
            board[block.y][block.x] = current.color;
        }
    }

    EventLinesClear();
    EventPieceSpawn();

    if (!IsPieceValid(current))
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::EventLinesClear()
{
    Board newBoard = {};
    int cleared = 0;
    int destinationY = GRID_HEIGHT - 1;

    for (int y = GRID_HEIGHT - 1; y >= 0; y--)
    {
        if (std::ranges::all_of(board[y], [](auto color) { return color.has_value(); }))
        {
            cleared++;
        }
        else
        {
            newBoard[destinationY] = board[y];
            destinationY--;
        }
    }

    if (cleared > 0)
    {
        board = newBoard;
        stats.score += cleared * STATS_SCORE_PER_LINE;
        stats.scoreText.setString("Score: " + std::to_string(stats.score));
    }
}

Piece Game::GenerateRandomPiece() const
{
    Piece piece;

    piece.type = ctx.random.Int(0, PIECE_TYPE_COUNT - 1);
    piece.rotation = 0;
    piece.position = {(GRID_WIDTH / 2) - 2, 0};
    piece.color = ctx.random.Color(sf::Color::Black, sf::Color::White);

    return piece;
}

sf::Vector2i Game::GetPointRotated(int type, int index, int rotation) const
{
    sf::Vector2f point(PIECE_SHAPES[type][index]);
    sf::Vector2f pivot = PIECE_PIVOTS[type];
    sf::Vector2f origin = point - pivot;

    sf::Vector2f rotated;
    switch (rotation)
    {
        case 0: rotated = origin; break;
        case 1: rotated = {-origin.y, origin.x}; break;
        case 2: rotated = {-origin.x,-origin.y}; break;
        case 3: rotated = { origin.y,-origin.x}; break;
    }
    rotated += pivot;

    return {int(std::round(rotated.x)), int(std::round(rotated.y))};
}

sf::Vector2f Game::GetBoardOrigin() const
{
    return {(gConfig.windowSize.x - GRID_WIDTH  * BLOCK_SIZE) / 2,
            (gConfig.windowSize.y - GRID_HEIGHT * BLOCK_SIZE) / 2};
}

bool Game::IsPieceValid(const Piece& piece) const
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        sf::Vector2i block = GetPointRotated(piece.type, i, piece.rotation) + piece.position;

        if (block.x < 0 || block.x >= GRID_WIDTH ||
            block.y < 0 || block.y >= GRID_HEIGHT ||
            (block.y >= 0 && board[block.y][block.x].has_value()))
        {
            return false;
        }
    }

    return true;
}

void Game::Render() const
{
    RenderBoard();
    RenderPiece(current, GetBoardOrigin(), true);
    RenderPiece(next, {gConfig.windowSize.x - 5.5f * BLOCK_SIZE, BLOCK_SIZE * 2.5f}, false);

    ctx.renderer.Draw(stats.scoreText);
}

void Game::RenderBoard() const
{
    sf::Vector2f origin = GetBoardOrigin();

    sf::RectangleShape shape({BLOCK_SIZE, BLOCK_SIZE});
    shape.setOutlineColor(GRID_COLOR);
    shape.setOutlineThickness(-1);

    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            shape.setFillColor(board[y][x].value_or(sf::Color::Transparent));
            shape.setPosition(origin + sf::Vector2f(x * BLOCK_SIZE, y * BLOCK_SIZE));

            ctx.renderer.Draw(shape);
        }
    }
}

void Game::RenderPiece(const Piece& piece, sf::Vector2f origin, bool world) const
{
    sf::RectangleShape shape({BLOCK_SIZE, BLOCK_SIZE});
    shape.setFillColor(piece.color);

    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        sf::Vector2i block = GetPointRotated(piece.type, i, piece.rotation);
        if (world)
        {
            block += piece.position;
        }

        shape.setPosition(origin + sf::Vector2f(block.x * BLOCK_SIZE, block.y * BLOCK_SIZE));

        ctx.renderer.Draw(shape);
    }
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        actionCooldown.Stop();
        fallCooldown.Stop();
        softDropCooldown.Stop();
    }
    else
    {
        actionCooldown.Start();
        fallCooldown.Start();
        softDropCooldown.Start();
    }
}