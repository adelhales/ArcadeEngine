// Copyright (c) 2025 Adel Hales

#include "Runner.h"

using namespace Runner;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitPlayer();
    InitStats();
    InitGround();
}

void Game::InitPlayer()
{
    player.shape.setFillColor(PLAYER_COLOR);
}

void Game::InitStats()
{
    stats.distanceText.setFillColor(STATS_DISTANCE_TEXT_COLOR);
    stats.distanceText.setPosition({10, 10});
}

void Game::InitGround()
{
    ground.setFillColor(GROUND_COLOR);
    ground.setSize({gConfig.windowSize.x, GROUND_HEIGHT});
    ground.setPosition({0, gConfig.windowSize.y / 2});
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);

    BindInputs();

    obstacles.clear();

    StartPlayer();
    StartStats();

    EventObstacleSpawn();
}

void Game::BindInputs()
{
    ctx.input.Bind(Jump, Input::Keyboard{sf::Keyboard::Scan::Space});
    ctx.input.Bind(Jump, Input::Gamepad{GamepadButton::South});

    ctx.input.Bind(Crouch, Input::Keyboard{sf::Keyboard::Scan::LControl});
    ctx.input.Bind(Crouch, Input::Gamepad{GamepadButton::West});

    ctx.input.Bind(Fall, Input::Keyboard{sf::Keyboard::Scan::S});
    ctx.input.Bind(Fall, Input::Axis{sf::Joystick::Axis::Y});
}

void Game::StartPlayer()
{
    player.shape.setSize(PLAYER_SIZE);
    player.shape.setOrigin(player.shape.getGeometricCenter());

    float positionY = ground.getPosition().y - player.shape.getSize().y / 2;
    player.shape.setPosition({gConfig.windowSize.x / 6, positionY});

    player.velocity = {};
    player.grounded = true;
    player.crouching = false;
}

void Game::StartStats()
{
    stats.speed = STATS_SPEED_START;
    stats.obstacleNextX = ctx.random.Float(OBSTACLE_MIN_DISTANCE, OBSTACLE_MAX_DISTANCE);

    stats.distance = 0;
    stats.distanceText.setString("Distance: 0");
}

void Game::Update()
{
    if (ctx.input.Pressed(Jump) && player.grounded && !player.crouching)
    {
        EventPlayerJump();
    }

    UpdatePlayer();
    UpdateObstacles();
    UpdateStats();

    HandleCollisions();
}

void Game::UpdatePlayer()
{
    UpdatePlayerMove();
    UpdatePlayerSize();
    UpdatePlayerGrounded();
}

void Game::UpdatePlayerMove()
{
    float gravity = PLAYER_GRAVITY;

    if (ctx.input.Pressed(Fall) && !player.grounded)
    {
        gravity *= PLAYER_FALL_FACTOR;
    }

    player.velocity.y += gravity * ctx.time.GetDeltaTime();
    player.shape.move({0, player.velocity.y * ctx.time.GetDeltaTime()});
}

void Game::UpdatePlayerSize()
{
    bool crouching = ctx.input.Pressed(Crouch);

    if (crouching == player.crouching || !player.grounded)
    {
        return;
    }

    float oldHeight = player.shape.getSize().y;
    float newHeight = crouching ? PLAYER_SIZE.y / 2 : PLAYER_SIZE.y;

    player.shape.setSize({player.shape.getSize().x, newHeight});
    player.shape.setOrigin(player.shape.getGeometricCenter());
    player.shape.move({0, (oldHeight - newHeight) / 2});

    player.crouching = crouching;
}

void Game::UpdatePlayerGrounded()
{
    float halfHeight = player.shape.getSize().y / 2;

    if (player.shape.getPosition().y + halfHeight >= ground.getPosition().y)
    {
        player.shape.setPosition({player.shape.getPosition().x, ground.getPosition().y - halfHeight});
        player.velocity.y = 0;
        player.grounded = true;
    }
    else
    {
        player.grounded = false;
    }
}

void Game::UpdateObstacles()
{
    UpdateObstaclesSpawn();
    UpdateObstaclesMove();
    UpdateObstaclesPopFront();
}

void Game::UpdateObstaclesSpawn()
{
    bool shouldObstacleSpawn = obstacles.back().shape.getPosition().x < gConfig.windowSize.x - stats.obstacleNextX;

    if (obstacles.empty() || shouldObstacleSpawn)
    {
        EventObstacleSpawn();
        stats.obstacleNextX = ctx.random.Float(OBSTACLE_MIN_DISTANCE, OBSTACLE_MAX_DISTANCE);
    }
}

void Game::UpdateObstaclesMove()
{
    for (auto& obstacle : obstacles)
    {
        obstacle.shape.move({-stats.speed * ctx.time.GetDeltaTime(), 0});
    }
}

void Game::UpdateObstaclesPopFront()
{
    const Obstacle& front = obstacles.front();

    bool isOutsideWindowLeft = front.shape.getPosition().x + front.shape.getSize().x < 0;
    if (isOutsideWindowLeft)
    {
        obstacles.pop_front();
    }
}

void Game::UpdateStats()
{
    stats.speed = std::min(stats.speed + STATS_SPEED_INCREASE * ctx.time.GetDeltaTime(), STATS_SPEED_MAX);

    stats.distance += stats.speed * ctx.time.GetDeltaTime();
    stats.distanceText.setString("Distance: " + std::to_string((int)stats.distance / 100));
}

void Game::EventPlayerJump()
{
    player.velocity.y = PLAYER_JUMP_VELOCITY;
    player.grounded = false;
}

void Game::EventObstacleSpawn()
{
    auto& obstacle = obstacles.emplace_back();

    float height = ctx.random.Float(OBSTACLE_MIN_HEIGHT, OBSTACLE_MAX_HEIGHT);
    obstacle.shape.setSize({OBSTACLE_WIDTH, height});

    if (ctx.random.Bool(OBSTACLE_AIR_PROBABILITY))
    {
        obstacle.shape.setFillColor(OBSTACLE_COLOR_HIGH);
        obstacle.shape.setPosition({gConfig.windowSize.x, ground.getPosition().y - PLAYER_SIZE.y / 2 - height});
    }
    else
    {
        obstacle.shape.setFillColor(OBSTACLE_COLOR_GROUND);
        obstacle.shape.setPosition({gConfig.windowSize.x, ground.getPosition().y - height});
    }
}

void Game::HandleCollisions()
{
    HandleCollisionsPlayerObstacles();
}

void Game::HandleCollisionsPlayerObstacles()
{
    for (const auto& obstacle : obstacles)
    {
        if (Intersects(player.shape, obstacle.shape))
        {
            LOG_INFO("You Lose!");
            ctx.scenes.RestartCurrentScene();
            return;
        }
    }
}

void Game::Render() const
{
    ctx.renderer.Draw(ground);

    for (const auto& obstacle : obstacles)
    {
        ctx.renderer.Draw(obstacle.shape);
    }

    ctx.renderer.Draw(player.shape);

    ctx.renderer.Draw(stats.distanceText);
}